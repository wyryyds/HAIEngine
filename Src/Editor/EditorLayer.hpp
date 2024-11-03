#pragma once
#include "HAIEngine.hpp"
#include "Core/Layer.hpp"
#include "Editor/ImSequence.hpp"
#include "Editor/ImCurveEdit.hpp"
#include "imgui_internal.h"
#include "SceneHierarchyPanel.hpp"
#include "ECS/Components/MeshFilter.hpp"
#include <iostream>
#include <string>
#include <chrono>

namespace HAIEngine
{

	struct Timer
	{
		std::string name;
		std::chrono::high_resolution_clock::time_point start;

		Timer(const std::string& name) : name(name), start(std::chrono::high_resolution_clock::now()) {}
		~Timer()
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::cout << name << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
		}
		void Log() const
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::cout << name << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
		}
	};

    static const char* SequencerItemTypeNames[] = { "Camera","Music", "ScreenEffect", "FadeIn", "Animation" };

    struct MySequence : public ImSequence::SequenceInterface
    {
        // interface with sequencer

        virtual int GetFrameMin() const  override { return mFrameMin; }
        virtual int GetFrameMax() const  override { return mFrameMax; }
        virtual int GetItemCount() const  override { return static_cast<int>(myItems.size()); }

        virtual int GetItemTypeCount() const override { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
        virtual const char* GetItemTypeName(int typeIndex) const override { return SequencerItemTypeNames[typeIndex]; }
        virtual const char* GetItemLabel(int index) const override
        {
            static char tmps[512];
            snprintf(tmps, 512, "[%02d] %s", index, SequencerItemTypeNames[myItems[index].mType]);
            return tmps;
        }

        virtual void Get(int index, int** start, int** end, int* type, unsigned int* color) override
        {
            MySequenceItem& item = myItems[index];
            if (color)
                *color = 0xFFAA8080; // same color for everyone, return color based on type
            if (start)
                *start = &item.mFrameStart;
            if (end)
                *end = &item.mFrameEnd;
            if (type)
                *type = item.mType;
        }
        virtual void Add(int type) override { myItems.push_back(MySequenceItem{ type, 0, 10, false }); };
        virtual void Del(int index) override { myItems.erase(myItems.begin() + index); }
        virtual void Duplicate(int index) override { myItems.push_back(myItems[index]); }

        virtual size_t GetCustomHeight(int index) override { return myItems[index].mExpanded ? 300 : 0; }

        // my datas
        MySequence() : mFrameMin(0), mFrameMax(0) {}
        int mFrameMin, mFrameMax;
        struct MySequenceItem
        {
            int mType;
            int mFrameStart, mFrameEnd;
            bool mExpanded;
        };
        std::vector<MySequenceItem> myItems;
        ImCurveEdit::RampEdit rampEdit;

        virtual void DoubleClick(int index) {
            if (myItems[index].mExpanded)
            {
                myItems[index].mExpanded = false;
                return;
            }
            for (auto& item : myItems)
                item.mExpanded = false;
            myItems[index].mExpanded = !myItems[index].mExpanded;
        }

        virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
        {
            static const char* labels[] = { "Translation", "Rotation" , "Scale" };

            rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
            rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
            draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
            for (int i = 0; i < 3; i++)
            {
                ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
                ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
                draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
                if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
                    rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
            }
            draw_list->PopClipRect();

            ImGui::SetCursorScreenPos(rc.Min);
            ImVec2 DisRect{ rc.Max.x - rc.Min.x, rc.Max.y - rc.Min.y };
            ImCurveEdit::Edit(rampEdit, DisRect, 137 + index, &clippingRect);
        }

        virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
        {
            rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
            rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
            draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < rampEdit.mPointCount[i]; j++)
                {
                    float p = rampEdit.mPts[i][j].x;
                    if (p < myItems[index].mFrameStart || p > myItems[index].mFrameEnd)
                        continue;
                    float r = (p - mFrameMin) / float(mFrameMax - mFrameMin);
                    float x = ImLerp(rc.Min.x, rc.Max.x, r);
                    draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
                }
            }
            draw_list->PopClipRect();
        }
    };

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		void CheckInput(TimeStep ts);
	
	private:
		ShaderLibrary m_ShaderLibrary;

		EditorCamera* m_PerspectiveCamera;
		std::unique_ptr<CameraController> m_CameraController;

		glm::vec3 m_LightColor{ 1.0f, 1.0f, 1.0f };
		glm::vec3 m_CubeColor{ 1.0f, 0.5f, 0.31f };
		glm::vec3 lightPos{ 1.2f, 2.0f, 2.0f };
		int m_Specuness = 64;

		std::shared_ptr<VertexBuffer> m_SuareVB, m_LightVB, m_testVB, m_skyboxVB;
		std::shared_ptr<VertexArray> m_SquareVA, m_LightVA, m_testVA, m_skyboxVA;

		std::shared_ptr<Texture2D> m_Texture, m_specularTexture;
		std::shared_ptr<Texture3D> m_skybox;

		std::shared_ptr<FrameBuffer> m_MSAAFrameBuffer, m_screenFrameBuffer, m_depthMap;

		float lastMouseX, lastMouseY;
		bool IsReControlMouse = false;
		glm::vec2 m_viewportSize = { 1920, 1080 };

		//sample ECS
		std::shared_ptr<Scene> scene;
		SceneHierarchyPanel m_panel;

		MeshFilter m_meshFilter;
		MeshRenderer m_meshRenderer;

        // timeline test
        MySequence mySequence;
	};
}