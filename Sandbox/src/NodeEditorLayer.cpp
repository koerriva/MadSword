
#include "NodeEditorLayer.h"
#include <fstream>

namespace App {
    NodeEditor::NodeEditor()
        :nodes_(),links_(),current_id_(0)
    {}
    NodeEditor::~NodeEditor()
    {
        MS_TRACE("Save Data...");
        Save();
    }
    void NodeEditor::Show()
	{
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        static bool open = true;
        ImGui::Begin(u8"NodeEditor",&open,ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "O")) {

                }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt+F4")) {
                    MS_ERROR("Quit.");
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("AddNode", "A")) {
                    //do something
                }
                if (ImGui::MenuItem("DelNode", "Delete/D")) {
                    //do something
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About", "F1")) {
                    m_OpenAboutWind = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (ImGui::IsKeyPressed(MadSword::Key::F1, false)) {
            m_OpenAboutWind = !m_OpenAboutWind;
        }
        if (m_OpenAboutWind) {
            ImGui::Begin("About", &m_OpenAboutWind);
            ImGui::Text("NodeEditor 0.1");
            ImGui::End();
        }
        

        ImNodes::BeginNodeEditor();

        if (ImGui::IsKeyPressed(MadSword::Key::A, false)) {
            AddNode();
        }

        for (Node& node : nodes_)
        {
            ImNodes::BeginNode(node.id);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("node");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(node.id << 8);
            ImGui::TextUnformatted("input");
            ImNodes::EndInputAttribute();

            ImNodes::BeginStaticAttribute(node.id << 16);
            ImGui::PushItemWidth(120.f);
            ImGui::DragFloat("value", &node.value, 0.01f);
            ImGui::PopItemWidth();
            ImNodes::EndStaticAttribute();

            ImNodes::BeginOutputAttribute(node.id << 24);
            const float text_width = ImGui::CalcTextSize("output").x;
            ImGui::Indent(120.f + ImGui::CalcTextSize("value").x - text_width);
            ImGui::TextUnformatted("output");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();
        }

        for (const Link& link : links_)
        {
            ImNodes::Link(link.id, link.start_attr, link.end_attr);
        }

        ImNodes::EndNodeEditor();

        if (ImGui::IsKeyPressed(MadSword::Key::Delete, false)) {
            RemoveNode();
        }

        {
            Link link;
            if (ImNodes::IsLinkCreated(&link.start_attr, &link.end_attr))
            {
                link.id = ++current_id_;
                links_.push_back(link);
            }
        }

        {
            int link_id;
            if (ImNodes::IsLinkDestroyed(&link_id))
            {
                auto iter =
                    std::find_if(links_.begin(), links_.end(), [link_id](const Link& link) -> bool {
                    return link.id == link_id;
                        });
                assert(iter != links_.end());
                links_.erase(iter);
            }
        }

        ImGui::End();
	}
	void NodeEditor::Save()
	{
        // Save the internal imnodes state
        ImNodes::SaveCurrentEditorStateToIniFile("save_load.ini");

        // Dump our editor state as bytes into a file

        std::fstream fout(
            "save_load.bytes", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

        // copy the node vector to file
        const size_t num_nodes = nodes_.size();
        fout.write(
            reinterpret_cast<const char*>(&num_nodes),
            static_cast<std::streamsize>(sizeof(size_t)));
        fout.write(
            reinterpret_cast<const char*>(nodes_.data()),
            static_cast<std::streamsize>(sizeof(Node) * num_nodes));

        // copy the link vector to file
        const size_t num_links = links_.size();
        fout.write(
            reinterpret_cast<const char*>(&num_links),
            static_cast<std::streamsize>(sizeof(size_t)));
        fout.write(
            reinterpret_cast<const char*>(links_.data()),
            static_cast<std::streamsize>(sizeof(Link) * num_links));
    	
        // copy the current_id to file
        fout.write(
            reinterpret_cast<const char*>(&current_id_), static_cast<std::streamsize>(sizeof(int)));
	}
	void NodeEditor::Load()
	{
        // Load the internal imnodes state
        ImNodes::LoadCurrentEditorStateFromIniFile("save_load.ini");

        // Load our editor state into memory

        std::fstream fin("save_load.bytes", std::ios_base::in | std::ios_base::binary);

        if (!fin.is_open())
        {
            return;
        }

        // copy nodes into memory
        size_t num_nodes;
        fin.read(reinterpret_cast<char*>(&num_nodes), static_cast<std::streamsize>(sizeof(size_t)));
        nodes_.resize(num_nodes);
        fin.read(
            reinterpret_cast<char*>(nodes_.data()),
            static_cast<std::streamsize>(sizeof(Node) * num_nodes));

        // copy links into memory
        size_t num_links;
        fin.read(reinterpret_cast<char*>(&num_links), static_cast<std::streamsize>(sizeof(size_t)));
        links_.resize(num_links);
        fin.read(
            reinterpret_cast<char*>(links_.data()),
            static_cast<std::streamsize>(sizeof(Link) * num_links));

        // copy current_id into memory
        fin.read(reinterpret_cast<char*>(&current_id_), static_cast<std::streamsize>(sizeof(int)));
	}

    void NodeEditor::AddNode()
    {
        //if (MadSword::Input::IsKeyDown(MadSword::Key::A)) {
        //    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        //        ImNodes::IsEditorHovered())
        //    {

        //    }
        //}
        const int node_id = ++current_id_;
        ImNodes::SetNodeScreenSpacePos(node_id, ImGui::GetMousePos());
        nodes_.push_back(Node(node_id, 0.f));
    }

    void NodeEditor::RemoveNode() {
        int selectedNodesNum = ImNodes::NumSelectedNodes();
        
        if (selectedNodesNum == 0)return;
        std::vector<int> nodesId;
        nodesId.resize(selectedNodesNum);
        ImNodes::GetSelectedNodes(nodesId.data());
        
        for each (int nodeId in nodesId)
        {
            auto it = nodes_.begin();
            while (it != nodes_.end()) {
                if (it->id == nodeId) {
                    it = nodes_.erase(it);
                }
                else {
                    it++;
                }
            }

            auto link_it = links_.begin();
            while (link_it != links_.end()) {
                int start_node = link_it->start_attr >> 24 & 0xFF;
                int end_node = link_it->end_attr >> 8 & 0xFF;

                if (start_node == nodeId || end_node == nodeId) {
                    link_it = links_.erase(link_it);
                }
                else {
                    link_it++;
                }
            }
        }

        int selectedLinksNum = ImNodes::NumSelectedLinks();
        if (selectedLinksNum == 0)return;
        std::vector<int> linksId;
        linksId.resize(selectedLinksNum);
        ImNodes::GetSelectedLinks(linksId.data());

        for each (int linkId in linksId) 
        {
            auto it = links_.begin();
            while (it != links_.end()) {
                if (it->id == linkId) {
                    it = links_.erase(it);
                }
                else {
                    it++;
                }
            }
        }
    }
}