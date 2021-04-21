#pragma once

#include <MadSword/Input.h>
#include <MadSword/KeyCodes.h>
#include <MadSword/MouseCodes.h>
#include <MadSword/Events/Event.h>
#include <MadSword/Events/KeyEvent.h>

#include <vector>
#include <imgui.h>
#include <imnodes/imnodes.h>

namespace App {
	struct Node
	{
		int   id;
		float value;

		Node() = default;

		Node(const int i, const float v) : id(i), value(v) {}
	};

	struct Link
	{
		int id;
		int start_attr, end_attr;
	};

	class NodeEditor {
	public:
		NodeEditor();
		~NodeEditor();
		void Show();
		void Save();
		void Load();
	private:
		void AddNode();
		void RemoveNode();
	private:
		std::vector<Node> nodes_;
		std::vector<Link> links_;
		int              current_id_;

		bool				m_OpenAboutWind = false;
	};

	void NodeEditorInitialize();

	void NodeEditorShow();

	void NodeEditorSave();

	void NodeEditorLoad();

	void NodeEditorShutdown();
}