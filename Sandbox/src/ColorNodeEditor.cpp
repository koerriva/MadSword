#include "imnodes/graph.h"
#include <imnodes/imnodes.h>
#include <imgui.h>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <vector>
#include <fstream>

#include <json/json.hpp>
using json = nlohmann::json;

#include <MadSword/KeyCodes.h>
#include <MadSword/MouseCodes.h>
#include <MadSword/Log.h>

namespace App
{
    namespace Demo
    {
        enum class NodeType
        {
            add,
            multiply,
            output,
            sine,
            time,
            value,
            input,
            neural,
        	neural_out,
        };

        struct Node
        {
            NodeType type;
            float    value;
            float    weight;
            int      synapse[8] = { 0 };
            int      output[2] = { 0 };

            Node() :type(NodeType::value), value(0.0f), weight(1.0f) {}

            Node(const NodeType t) : type(t), value(0.0f), weight(1.0f) {}

            Node(const NodeType t, const float v) : type(t), value(v), weight(1.0f) {}

            Node(const NodeType t, const float v, const float w) :type(t), value(v), weight(w) {}
        };

        template<class T>
        T clamp(T x, T a, T b)
        {
            return std::min(b, std::max(x, a));
        }

        static float current_time_seconds = 0.f;
        static bool  emulate_three_button_mouse = false;

        ImU32 evaluate(const Graph<Node>& graph, const int root_node)
        {
            std::stack<int> postorder;
            dfs_traverse(
                graph, root_node, [&postorder](const int node_id) -> void { postorder.push(node_id); });

            std::stack<float> value_stack;
            while (!postorder.empty())
            {
                const int id = postorder.top();
                postorder.pop();

                switch (const Node & node = graph.node(id); node.type)
                {
                case NodeType::add:
                {
                    const auto rhs = value_stack.top();
                    value_stack.pop();
                    const auto lhs = value_stack.top();
                    value_stack.pop();
                    value_stack.push(lhs + rhs);
                }
                break;
                case NodeType::multiply:
                {
                    const auto rhs = value_stack.top();
                    value_stack.pop();
                    const auto lhs = value_stack.top();
                    value_stack.pop();
                    value_stack.push(lhs + rhs);
                }
                break;
                case NodeType::sine:
                {
                    const auto x = value_stack.top();
                    value_stack.pop();
                    const float res = std::abs(std::sin(x));
                    value_stack.push(res);
                }
                break;
                case NodeType::time:
                {
                    value_stack.push(current_time_seconds);
                }
                break;
                case NodeType::input:
                {
                    const float in = node.value;
                    value_stack.push(in);
                }
                break;
                case NodeType::neural:
                {
                	
	                const size_t len = 8;
                    float output = 0.0f;
                	
                    for (size_t i = 0; i < len; i++)
                    {
                        const auto x = value_stack.top();
                        value_stack.pop();

                        const Node& synapse = graph.node(node.synapse[len-i-1]);
                    	
                        output += x * synapse.weight;
                    }

	                for (int i = 0; i < 2; ++i)
	                {
                        value_stack.push(output);
	                }
                    
                }
                break;
                case NodeType::neural_out:
	            {
                    //const auto x = value_stack.top();
                    value_stack.pop();
                }break;
                case NodeType::value:
                {
                    // If the edge does not have an edge connecting to another node, then just use the value
                    // at this node. It means the node's input pin has not been connected to anything and
                    // the value comes from the node's UI.
                    if (graph.num_edges_from_node(id) == 0ull)
                    {
                        value_stack.push(node.value);
                    }
                }
                break;
                default:
                    break;
                }
            }

            // The final output node isn't evaluated in the loop -- instead we just pop
            // the three values which should be in the stack.
            assert(value_stack.size() == 3ull);
            const int b = static_cast<int>(clamp(value_stack.top(), 0.f, 1.f) * 255.f + 0.5f);
            value_stack.pop();
            const int g = static_cast<int>(clamp(value_stack.top(), 0.f, 1.f) * 255.f + 0.5f);
            value_stack.pop();
            const int r = static_cast<int>(clamp(value_stack.top(), 0.f, 1.f) * 255.f + 0.5f);
            value_stack.pop();

            return IM_COL32(r, g, b, 255);
        }

        class ColorNodeEditor
        {
        public:
            ColorNodeEditor() : graph_(), nodes_(), root_node_id_(-1) {}
            ~ColorNodeEditor() = default;
            void show()
            {
                //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

                ImGuiIO& io = ImGui::GetIO();
                
                // Update timer context
                current_time_seconds += io.DeltaTime;

                // The node editor window
                ImGui::Begin("Neural Editor");
                ImGui::TextUnformatted("Edit the color of the output color window using nodes.");
                ImGui::Columns(2);
                ImGui::TextUnformatted("A -- add node");
                ImGui::TextUnformatted("X -- delete selected node or link");
                ImGui::NextColumn();
                if (ImGui::Checkbox("emulate_three_button_mouse", &emulate_three_button_mouse))
                {
                    ImNodes::GetIO().EmulateThreeButtonMouse.Modifier =
                        emulate_three_button_mouse ? &ImGui::GetIO().KeyAlt : NULL;
                }
                ImGui::Columns(1);

                ImNodes::BeginNodeEditor();

                // Handle new nodes
                // These are driven by the user, so we place this code before rendering the nodes
                {
                    const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                        ImNodes::IsEditorHovered() &&
                        (ImGui::IsKeyReleased(MadSword::Key::A)||ImGui::IsMouseClicked(MadSword::Mouse::Button1));

                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
                    if (!ImGui::IsAnyItemHovered() && open_popup)
                    {
                        ImGui::OpenPopup("add node");
                    }

                    if (ImGui::BeginPopup("add node"))
                    {
                        const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

                        if (ImGui::MenuItem("add"))
                        {
                            const Node value(NodeType::value, 0.f);
                            const Node op(NodeType::add);

                            UiNode ui_node;
                            ui_node.type = UiNodeType::add;
                            ui_node.add.lhs = graph_.insert_node(value);
                            ui_node.add.rhs = graph_.insert_node(value);
                            ui_node.id = graph_.insert_node(op);

                            graph_.insert_edge(ui_node.id, ui_node.add.lhs);
                            graph_.insert_edge(ui_node.id, ui_node.add.rhs);

                            nodes_.push_back(ui_node);
                            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                        }

                        if (ImGui::MenuItem("multiply"))
                        {
                            const Node value(NodeType::value, 0.f);
                            const Node op(NodeType::multiply);

                            UiNode ui_node;
                            ui_node.type = UiNodeType::multiply;
                            ui_node.multiply.lhs = graph_.insert_node(value);
                            ui_node.multiply.rhs = graph_.insert_node(value);
                            ui_node.id = graph_.insert_node(op);

                            graph_.insert_edge(ui_node.id, ui_node.multiply.lhs);
                            graph_.insert_edge(ui_node.id, ui_node.multiply.rhs);

                            nodes_.push_back(ui_node);
                            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                        }

                        if (ImGui::MenuItem("output") && root_node_id_ == -1)
                        {
                            const Node value(NodeType::value, 0.f);
                            const Node out(NodeType::output);

                            UiNode ui_node;
                            ui_node.type = UiNodeType::output;
                            ui_node.output.r = graph_.insert_node(value);
                            ui_node.output.g = graph_.insert_node(value);
                            ui_node.output.b = graph_.insert_node(value);
                            ui_node.id = graph_.insert_node(out);

                            graph_.insert_edge(ui_node.id, ui_node.output.r);
                            graph_.insert_edge(ui_node.id, ui_node.output.g);
                            graph_.insert_edge(ui_node.id, ui_node.output.b);

                            nodes_.push_back(ui_node);
                            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                            root_node_id_ = ui_node.id;
                        }

                        if (ImGui::MenuItem("sine"))
                        {
                            const Node value(NodeType::value, 0.f);
                            const Node op(NodeType::sine);

                            UiNode ui_node;
                            ui_node.type = UiNodeType::sine;
                            ui_node.sine.input = graph_.insert_node(value);
                            ui_node.id = graph_.insert_node(op);

                            graph_.insert_edge(ui_node.id, ui_node.sine.input);

                            nodes_.push_back(ui_node);
                            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                        }

                        if (ImGui::MenuItem("time"))
                        {
                            UiNode ui_node;
                            ui_node.type = UiNodeType::time;
                            ui_node.id = graph_.insert_node(Node(NodeType::time));

                            nodes_.push_back(ui_node);
                            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                        }

                        if (ImGui::MenuItem("input"))
                        {
                            UiNode ui_node;
                            ui_node.type = UiNodeType::input;
                            ui_node.id = graph_.insert_node(Node(NodeType::input));

                            nodes_.push_back(ui_node);
                            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                        }

                        if (ImGui::MenuItem("neural"))
                        {
                            const Node value(NodeType::value);
                            const Node out(NodeType::neural_out);
                            Node op(NodeType::neural);

                            UiNode ui_node;
                            ui_node.type = UiNodeType::neural;
                            
                            size_t len = sizeof(ui_node.neural.synapse) / sizeof(int);
                            for (size_t i = 0; i < len; i++)
                            {
                                int synapseId = graph_.insert_node(value);
                                ui_node.neural.synapse[i] = synapseId;
                                ui_node.neural.synapse_weight[i] = 0;
                                op.synapse[i] = synapseId;
                            }
                        	
                        	for (size_t i=0;i<2;i++)
                        	{
                                int outId = graph_.insert_node(out);
                                ui_node.neural.output[i] = outId;
                                op.output[i] = outId;
                        	}
                            
                            ui_node.id = graph_.insert_node(op);

                            for (size_t i = 0; i < len; ++i)
                            {
                                int from = ui_node.id;
                                int to = op.synapse[i];
                                graph_.insert_edge(from, to);
                            }

                        	for (size_t i=0;i<2;i++)
                        	{
                                int from = op.output[i];
                                int to = ui_node.id;
                                graph_.insert_edge(from, to);
                        	}
                            
                            nodes_.push_back(ui_node);
                            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                        }

                        ImGui::EndPopup();
                    }
                    ImGui::PopStyleVar();
                }

                for (UiNode& node : nodes_)
                {
                    switch (node.type)
                    {
                    case UiNodeType::add:
                    {
                        const float node_width = 100.f;
                        ImNodes::BeginNode(node.id);

                        ImNodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted("add");
                        ImNodes::EndNodeTitleBar();
                        {
                            ImNodes::BeginInputAttribute(node.add.lhs);
                            const float label_width = ImGui::CalcTextSize("left").x;
                            ImGui::TextUnformatted("left");
                            if (graph_.num_edges_from_node(node.add.lhs) == 0ull)
                            {
                                ImGui::SameLine();
                                ImGui::PushItemWidth(node_width - label_width);
                                ImGui::DragFloat("##hidelabel", &graph_.node(node.add.lhs).value, 0.01f);
                                ImGui::PopItemWidth();
                            }
                            ImNodes::EndInputAttribute();
                        }

                        {
                            ImNodes::BeginInputAttribute(node.add.rhs);
                            const float label_width = ImGui::CalcTextSize("right").x;
                            ImGui::TextUnformatted("right");
                            if (graph_.num_edges_from_node(node.add.rhs) == 0ull)
                            {
                                ImGui::SameLine();
                                ImGui::PushItemWidth(node_width - label_width);
                                ImGui::DragFloat("##hidelabel", &graph_.node(node.add.rhs).value, 0.01f);
                                ImGui::PopItemWidth();
                            }
                            ImNodes::EndInputAttribute();
                        }

                        ImGui::Spacing();

                        {
                            ImNodes::BeginOutputAttribute(node.id);
                            const float label_width = ImGui::CalcTextSize("result").x;
                            ImGui::Indent(node_width - label_width);
                            ImGui::TextUnformatted("result");
                            ImNodes::EndOutputAttribute();
                        }

                        ImNodes::EndNode();
                    }
                    break;
                    case UiNodeType::multiply:
                    {
                        const float node_width = 100.0f;
                        ImNodes::BeginNode(node.id);

                        ImNodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted("multiply");
                        ImNodes::EndNodeTitleBar();

                        {
                            ImNodes::BeginInputAttribute(node.multiply.lhs);
                            const float label_width = ImGui::CalcTextSize("left").x;
                            ImGui::TextUnformatted("left");
                            if (graph_.num_edges_from_node(node.multiply.lhs) == 0ull)
                            {
                                ImGui::SameLine();
                                ImGui::PushItemWidth(node_width - label_width);
                                ImGui::DragFloat(
                                    "##hidelabel", &graph_.node(node.multiply.lhs).value, 0.01f);
                                ImGui::PopItemWidth();
                            }
                            ImNodes::EndInputAttribute();
                        }

                        {
                            ImNodes::BeginInputAttribute(node.multiply.rhs);
                            const float label_width = ImGui::CalcTextSize("right").x;
                            ImGui::TextUnformatted("right");
                            if (graph_.num_edges_from_node(node.multiply.rhs) == 0ull)
                            {
                                ImGui::SameLine();
                                ImGui::PushItemWidth(node_width - label_width);
                                ImGui::DragFloat(
                                    "##hidelabel", &graph_.node(node.multiply.rhs).value, 0.01f);
                                ImGui::PopItemWidth();
                            }
                            ImNodes::EndInputAttribute();
                        }

                        ImGui::Spacing();

                        {
                            ImNodes::BeginOutputAttribute(node.id);
                            const float label_width = ImGui::CalcTextSize("result").x;
                            ImGui::Indent(node_width - label_width);
                            ImGui::TextUnformatted("result");
                            ImNodes::EndOutputAttribute();
                        }

                        ImNodes::EndNode();
                    }
                    break;
                    case UiNodeType::output:
                    {
                        const float node_width = 100.0f;
                        ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(11, 109, 191, 255));
                        ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(45, 126, 194, 255));
                        ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(81, 148, 204, 255));
                        ImNodes::BeginNode(node.id);

                        ImNodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted("output");
                        ImNodes::EndNodeTitleBar();

                        ImGui::Dummy(ImVec2(node_width, 0.f));
                        {
                            ImNodes::BeginInputAttribute(node.output.r);
                            const float label_width = ImGui::CalcTextSize("r").x;
                            ImGui::TextUnformatted("r");
                            if (graph_.num_edges_from_node(node.output.r) == 0ull)
                            {
                                ImGui::SameLine();
                                ImGui::PushItemWidth(node_width - label_width);
                                ImGui::DragFloat(
                                    "##hidelabel", &graph_.node(node.output.r).value, 0.01f, 0.f, 1.0f);
                                ImGui::PopItemWidth();
                            }
                            ImNodes::EndInputAttribute();
                        }

                        ImGui::Spacing();

                        {
                            ImNodes::BeginInputAttribute(node.output.g);
                            const float label_width = ImGui::CalcTextSize("g").x;
                            ImGui::TextUnformatted("g");
                            if (graph_.num_edges_from_node(node.output.g) == 0ull)
                            {
                                ImGui::SameLine();
                                ImGui::PushItemWidth(node_width - label_width);
                                ImGui::DragFloat(
                                    "##hidelabel", &graph_.node(node.output.g).value, 0.01f, 0.f, 1.f);
                                ImGui::PopItemWidth();
                            }
                            ImNodes::EndInputAttribute();
                        }

                        ImGui::Spacing();

                        {
                            ImNodes::BeginInputAttribute(node.output.b);
                            const float label_width = ImGui::CalcTextSize("b").x;
                            ImGui::TextUnformatted("b");
                            if (graph_.num_edges_from_node(node.output.b) == 0ull)
                            {
                                ImGui::SameLine();
                                ImGui::PushItemWidth(node_width - label_width);
                                ImGui::DragFloat(
                                    "##hidelabel", &graph_.node(node.output.b).value, 0.01f, 0.f, 1.0f);
                                ImGui::PopItemWidth();
                            }
                            ImNodes::EndInputAttribute();
                        }
                        ImNodes::EndNode();
                        ImNodes::PopColorStyle();
                        ImNodes::PopColorStyle();
                        ImNodes::PopColorStyle();
                    }
                    break;
                    case UiNodeType::sine:
                    {
                        const float node_width = 100.0f;
                        ImNodes::BeginNode(node.id);

                        ImNodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted("sine");
                        ImNodes::EndNodeTitleBar();

                        {
                            ImNodes::BeginInputAttribute(node.sine.input);
                            const float label_width = ImGui::CalcTextSize("number").x;
                            ImGui::TextUnformatted("number");
                            if (graph_.num_edges_from_node(node.sine.input) == 0ull)
                            {
                                ImGui::SameLine();
                                ImGui::PushItemWidth(node_width - label_width);
                                ImGui::DragFloat(
                                    "##hidelabel", &graph_.node(node.sine.input).value, 0.01f, 0.f, 1.0f);
                                ImGui::PopItemWidth();
                            }
                            ImNodes::EndInputAttribute();
                        }

                        ImGui::Spacing();

                        {
                            ImNodes::BeginOutputAttribute(node.id);
                            const float label_width = ImGui::CalcTextSize("output").x;
                            ImGui::Indent(node_width - label_width);
                            ImGui::TextUnformatted("output");
                            ImNodes::EndInputAttribute();
                        }

                        ImNodes::EndNode();
                    }
                    break;
                    case UiNodeType::time:
                    {
                        ImNodes::BeginNode(node.id);

                        ImNodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted("time");
                        ImNodes::EndNodeTitleBar();

                        ImNodes::BeginOutputAttribute(node.id);
                        ImGui::Text("output");
                        ImNodes::EndOutputAttribute();

                        ImNodes::EndNode();
                    }
                    break;
                    case UiNodeType::input:
                    {
                        const float node_width = 100.0f;
                        ImNodes::BeginNode(node.id);

                        ImNodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted("input");
                        ImNodes::EndNodeTitleBar();

                        ImNodes::BeginInputAttribute(node.id);

                        ImGui::PushItemWidth(node_width);
                        if(ImGui::DragFloat("##hidelabel", &graph_.node(node.id).value, 0.001f, 0.f, 1.0f))
                        {
                            MS_TRACE("change input {}",graph_.node(node.id).value);
                        }
                        ImGui::PopItemWidth();

                        ImNodes::EndInputAttribute();

                        ImGui::Spacing();

                        ImNodes::BeginOutputAttribute(node.id);
                        ImGui::Text("output");
                        ImNodes::EndOutputAttribute();

                        ImNodes::EndNode();
                    }
                    break;
                    case UiNodeType::neural:
                    {
                        const float node_width = 100.0f;
                        ImNodes::BeginNode(node.id);

                        ImNodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted("neural");
                        ImNodes::EndNodeTitleBar();

                        {
                            const float label_width = ImGui::CalcTextSize("synapse").x;
                        	
                            size_t len = sizeof(node.neural.synapse) / sizeof(int);
                            for (size_t i = 0; i < len; i++)
                            {
                                ImNodes::BeginInputAttribute(node.neural.synapse[i]);
                                ImGui::Text("synapse %d", i);
                                //if (graph_.num_edges_from_node(node.neural.synapse[i]) == 0ull)
                                //{
                                    ImGui::SameLine();
                                    ImGui::PushItemWidth(node_width - label_width);
                                    if(ImGui::DragFloat("##hidelabel", &graph_.node(node.neural.synapse[i]).weight, 0.001f, 0.f, 1.0f))
                                    {
                                        const float w = graph_.node(node.neural.synapse[i]).weight;
                                        node.neural.synapse_weight[i] = w;
                                        MS_TRACE("change synapse {} {}", i, w);
                                    }
                                    ImGui::PopItemWidth();
                                //}
                                ImNodes::EndInputAttribute();
                            }
                        }

                        ImGui::Spacing();

                        {
                            const float label_width = ImGui::CalcTextSize("output").x;

                            size_t len = sizeof(node.neural.output) / sizeof(int);
                            for (size_t i = 0; i < len; ++i)
                            {
                                ImNodes::BeginOutputAttribute(node.neural.output[i]);
                                ImGui::Indent(node_width - label_width);
                                ImGui::Text("output %d",i);
                                ImNodes::EndInputAttribute();
                            }
                        }

                        ImNodes::EndNode();
                    }
                    break;
                    }
                }

                for (const auto& edge : graph_.edges())
                {
                    // If edge doesn't start at value, then it's an internal edge, i.e.
                    // an edge which links a node's operation to its input. We don't
                    // want to render node internals with visible links.
                    if (graph_.node(edge.from).type != NodeType::value)
                        continue;

                    ImNodes::Link(edge.id, edge.from, edge.to);
                }

                ImNodes::EndNodeEditor();

                // Handle new links
                // These are driven by Imnodes, so we place the code after EndNodeEditor().

                {
                    int start_attr, end_attr;
                    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
                    {
                        const NodeType start_type = graph_.node(start_attr).type;
                        const NodeType end_type = graph_.node(end_attr).type;

                        const bool valid_link = start_type != end_type;
                        if (valid_link)
                        {
                            // Ensure the edge is always directed from the value to
                            // whatever produces the value
                            if (start_type != NodeType::value)
                            {
                                std::swap(start_attr, end_attr);
                            }
                            graph_.insert_edge(start_attr, end_attr);
                        }
                    }
                }

                // Handle deleted links

                {
                    int link_id;
                    if (ImNodes::IsLinkDestroyed(&link_id))
                    {
                        graph_.erase_edge(link_id);
                    }
                }

                {
                    const int num_selected = ImNodes::NumSelectedLinks();
                    if (num_selected > 0 && ImGui::IsKeyReleased(MadSword::Key::X))
                    {
                        static std::vector<int> selected_links;
                        selected_links.resize(static_cast<size_t>(num_selected));
                        ImNodes::GetSelectedLinks(selected_links.data());
                        for (const int edge_id : selected_links)
                        {
                            graph_.erase_edge(edge_id);
                        }
                    }
                }

                {
                    const int num_selected = ImNodes::NumSelectedNodes();
                    if (num_selected > 0 && ImGui::IsKeyReleased(MadSword::Key::X))
                    {
                        static std::vector<int> selected_nodes;
                        selected_nodes.resize(static_cast<size_t>(num_selected));
                        ImNodes::GetSelectedNodes(selected_nodes.data());
                        for (const int node_id : selected_nodes)
                        {
                            graph_.erase_node(node_id);
                            auto iter = std::find_if(
                                nodes_.begin(), nodes_.end(), [node_id](const UiNode& node) -> bool {
                                    return node.id == node_id;
                                });
                            // Erase any additional internal nodes
                            switch (iter->type)
                            {
                            case UiNodeType::add:
                                graph_.erase_node(iter->add.lhs);
                                graph_.erase_node(iter->add.rhs);
                                break;
                            case UiNodeType::multiply:
                                graph_.erase_node(iter->multiply.lhs);
                                graph_.erase_node(iter->multiply.rhs);
                                break;
                            case UiNodeType::output:
                                graph_.erase_node(iter->output.r);
                                graph_.erase_node(iter->output.g);
                                graph_.erase_node(iter->output.b);
                                root_node_id_ = -1;
                                break;
                            case UiNodeType::sine:
                                graph_.erase_node(iter->sine.input);
                                break;
                            case UiNodeType::neural:
                                graph_.erase_node(iter->sine.input);
                                break;
                            default:
                                break;
                            }
                            nodes_.erase(iter);
                        }
                    }
                }

                ImGui::End();

                // The color output window

                const ImU32 color =
                    root_node_id_ != -1 ? evaluate(graph_, root_node_id_) : IM_COL32(255, 20, 147, 255);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, color);
                ImGui::Begin("output color");
                ImGui::End();
                ImGui::PopStyleColor();

                if (ImGui::IsKeyPressed(MadSword::Key::F5))
                {
                    MS_TRACE("Save Editor Data...");
                    Save();
                }

            	if(ImGui::IsKeyPressed(MadSword::Key::F6))
            	{
                    MS_TRACE("Load Editor Data...");
                    Load();
            	}
            }

            void Save() {
                // Save the internal imnodes state
                ImNodes::SaveCurrentEditorStateToIniFile("neural_editor.ini");

                // Dump our editor state as bytes into a file

                std::fstream fout("neural_editor.bytes", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

                // copy the node vector to file
                const size_t num_nodes = nodes_.size();
                fout.write(reinterpret_cast<const char*>(&num_nodes),static_cast<std::streamsize>(sizeof(size_t)));
                fout.write(reinterpret_cast<const char*>(nodes_.data()),static_cast<std::streamsize>(sizeof(UiNode) * num_nodes));

                // Dump graph to file
            	
                graph_.Save(fout);

            	// copy root_node_id
                fout.write(reinterpret_cast<const char*>(&root_node_id_), static_cast<std::streamsize>(sizeof(int)));
            }

            void Load() {
                // Load the internal imnodes state
                ImNodes::LoadCurrentEditorStateFromIniFile("neural_editor.ini");

                // Load our editor state into memory
                std::fstream fin("neural_editor.bytes", std::ios_base::in | std::ios_base::binary);

                if (!fin.is_open())
                {
                    return;
                }

                // copy nodes into memory
                size_t num_nodes;
                fin.read(reinterpret_cast<char*>(&num_nodes), static_cast<std::streamsize>(sizeof(size_t)));
                nodes_.resize(num_nodes);
                fin.read(reinterpret_cast<char*>(nodes_.data()), static_cast<std::streamsize>(sizeof(UiNode) * num_nodes));

                // copy graph into memory
                graph_.Load(fin);

                // copy current_id into memory
                fin.read(reinterpret_cast<char*>(&root_node_id_), static_cast<std::streamsize>(sizeof(int)));
            }
        private:
            enum class UiNodeType
            {
                add,
                multiply,
                output,
                sine,
                time,
                input,
                neural,
            };

            struct UiNode
            {
                UiNodeType type;
                // The identifying id of the ui node. For add, multiply, sine, and time
                // this is the "operation" node id. The additional input nodes are
                // stored in the structs.
                int id;

                union
                {
                    struct
                    {
                        int lhs, rhs;
                    } add;

                    struct
                    {
                        int lhs, rhs;
                    } multiply;

                    struct
                    {
                        int r, g, b;
                    } output;

                    struct
                    {
                        int input;
                    } sine;

                	struct
                	{
                        int input;
                        float weight;
                	} cell;
                	
                    struct
                    {
                        int synapse[8];
                        int output[2];
                        float synapse_weight[8];
                        float weight;
                    } neural;
                };
            };

            Graph<Node>         graph_;
            std::vector<UiNode> nodes_;
            int                 root_node_id_;
        };

        static ColorNodeEditor color_editor;
    } // namespace

    void NodeEditorInitialize()
    {
        ImNodesIO& io = ImNodes::GetIO();
        io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    }

    void NodeEditorShow() { Demo::color_editor.show(); }

    void NodeEditorSave() { Demo::color_editor.Save(); }

    void NodeEditorLoad() { Demo::color_editor.Load(); }

    void NodeEditorShutdown() {}
} // namespace example
