/// <summary>
/// DxLib を入れたバージョン
///  2023/12/6 花井
/// </summary>

#include "global.h"
#include "RootObject.h"
#include "../ImGui/imgui_impl_dxlib.hpp"
#include <time.h>

namespace Screen {
	static const int WIDTH = 1280;
	static const int HEIGHT = 720;
	static const char* WINDOW_NAME = "Skribbl";
	static const int WINDOW_EXTEND = 0;
	static const int WINDOW_MODE = 1;
};

// エントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	srand((unsigned)time(NULL));

	SetGraphMode(Screen::WIDTH, Screen::HEIGHT, 32);
	SetOutApplicationLogValidFlag(FALSE); // ログを出さない
	SetMainWindowText(Screen::WINDOW_NAME);
	SetWindowSizeExtendRate(Screen::WINDOW_EXTEND);
	ChangeWindowMode(Screen::WINDOW_MODE); // Windowモードの場合

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	// ユーザーの入力をImGuiも扱えるようにする
	SetHookWinProc([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT /*CALLBACK*/
	{
		// DxLibとImGuiのウィンドウプロシージャを両立させる
		SetUseHookWinProcReturnValue(FALSE);
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	});

	// imgui init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui_ImplDXlib_Init();


	RootObject* pRootObject = new RootObject;
	pRootObject->Initialize();

	while (true) {
	//全オブジェクトの更新処理
	//ルートオブジェクトのUpdateを呼んだあと、自動的に子、孫のUpdateが呼ばれる
		ImGui_ImplDXlib_NewFrame();
		ImGui::NewFrame();

		pRootObject->UpdateSub();

		if (DxLib::ProcessMessage() == -1 /*|| AppIsExit()*/)
			break;

		//ClearDrawScreen();
		//全オブジェクトを描画
		//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
		pRootObject->DrawSub();

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDXlib_RenderDrawData();


		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		RefreshDxLibDirect3DSetting();

		ScreenFlip();
	}
	pRootObject->ReleaseSub();
	delete pRootObject;

	//Imguiの終了処理
	ImGui_ImplDXlib_Shutdown();
	ImGui::DestroyContext();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;
}
