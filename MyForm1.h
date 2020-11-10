#pragma once

#include <Windows.h>
#include "pch.h"
#include "interception.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>


#pragma comment(lib, "interception.lib")

int get_screen_width(void) {
	return GetSystemMetrics(SM_CXSCREEN);
}

int get_screen_height(void) {
	return GetSystemMetrics(SM_CYSCREEN);
}

struct point {
	double x;
	double y;
	point(double x, double y) : x(x), y(y) {}
};

inline bool is_color(int red, int green, int blue)
{
	if (green >= 190) {
		return false;
	}

	if (green >= 140) {
		return abs(red - blue) <= 8 &&
			red - green >= 50 &&
			blue - green >= 50 &&
			red >= 105 &&
			blue >= 105;
	}

	return abs(red - blue) <= 13 &&
		red - green >= 60 &&
		blue - green >= 60 &&
		red >= 110 &&
		blue >= 100;
}

float smooth = 0.4f;

BYTE* screenData = 0;
bool run_threads = true;
const int screen_width = get_screen_width(), screen_height = get_screen_height();

int aim_x = 0;
int aim_y = 0;
 
bool bot() {
	int w = 100, h = 100;
	auto t_start = std::chrono::high_resolution_clock::now();
	auto t_end = std::chrono::high_resolution_clock::now();

	HDC hScreen = GetDC(NULL);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
	screenData = (BYTE*)malloc(5 * screen_width * screen_height);
	HDC hDC = CreateCompatibleDC(hScreen);
	point middle_screen(screen_width / 2, screen_height / 2);

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = w;
	bmi.biHeight = -h;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;

	while (run_threads) {
		Sleep(6);
		HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
		BOOL bRet = BitBlt(hDC, 0, 0, w, h, hScreen, middle_screen.x - (w / 2), middle_screen.y - (h / 2), SRCCOPY);
		SelectObject(hDC, old_obj);
		GetDIBits(hDC, hBitmap, 0, h, screenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
		bool stop_loop = false;
		for (int j = 0; j < h; ++j) {
			for (int i = 0; i < w * 4; i += 4) {
#define red screenData[i + (j*w*4) + 2]
#define green screenData[i + (j*w*4) + 1]
#define blue screenData[i + (j*w*4) + 0]

				if (is_color(red, green, blue)) {
					aim_x = (i / 4) - (w / 2);
					aim_y = j - (h / 2) + 3;
					stop_loop = true;
					break;
				}
			}
			if (stop_loop) {
				break;
			}
		}
		if (!stop_loop) {
			aim_x = 0;
			aim_y = 0;
		}
	}
	return 0;
}

InterceptionContext context;
InterceptionDevice device;
InterceptionStroke stroke;

namespace Dll2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}







	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::ComponentModel::BackgroundWorker^ backgroundWorker1;

	private: System::Windows::Forms::RadioButton^ mouse5;
	private: System::Windows::Forms::RadioButton^ mouse4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::RadioButton^ mouse3;







	private: System::ComponentModel::IContainer^ components;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->mouse5 = (gcnew System::Windows::Forms::RadioButton());
			this->mouse4 = (gcnew System::Windows::Forms::RadioButton());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->mouse3 = (gcnew System::Windows::Forms::RadioButton());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(12, 30);
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(125, 45);
			this->trackBar1->TabIndex = 1;
			this->trackBar1->TickStyle = System::Windows::Forms::TickStyle::None;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &MyForm::trackBar1_Scroll);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(34, 59);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(83, 16);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Smoothness";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(143, 31);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(56, 16);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Value: 0";
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker1_DoWork);
			// 
			// mouse5
			// 
			this->mouse5->AutoSize = true;
			this->mouse5->Location = System::Drawing::Point(12, 7);
			this->mouse5->Name = L"mouse5";
			this->mouse5->Size = System::Drawing::Size(63, 17);
			this->mouse5->TabIndex = 5;
			this->mouse5->TabStop = true;
			this->mouse5->Text = L"Mouse5";
			this->mouse5->UseVisualStyleBackColor = true;
			this->mouse5->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton1_CheckedChanged);
			// 
			// mouse4
			// 
			this->mouse4->AutoSize = true;
			this->mouse4->Location = System::Drawing::Point(81, 7);
			this->mouse4->Name = L"mouse4";
			this->mouse4->Size = System::Drawing::Size(63, 17);
			this->mouse4->TabIndex = 6;
			this->mouse4->TabStop = true;
			this->mouse4->Text = L"Mouse4";
			this->mouse4->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(209, 73);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(42, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Aimbot:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::Maroon;
			this->label4->Location = System::Drawing::Point(250, 73);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(27, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"OFF";
			// 
			// mouse3
			// 
			this->mouse3->AutoSize = true;
			this->mouse3->Location = System::Drawing::Point(150, 7);
			this->mouse3->Name = L"mouse3";
			this->mouse3->Size = System::Drawing::Size(63, 17);
			this->mouse3->TabIndex = 9;
			this->mouse3->TabStop = true;
			this->mouse3->Text = L"Mouse3";
			this->mouse3->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(289, 95);
			this->Controls->Add(this->mouse3);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->mouse4);
			this->Controls->Add(this->mouse5);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->label4);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->ShowIcon = false;
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		std::thread(bot).detach();
		 
		backgroundWorker1->WorkerSupportsCancellation = true;
		backgroundWorker1->RunWorkerAsync();
	}

	private: System::Void checkBox1_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{

	}
private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e)
{
	 
}
private: System::Void trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	int trackBarValue = trackBar1->Value;
	smooth = trackBarValue;

	label2->Text = "Value: " + trackBar1->Value.ToString();
}
private: System::Void radioButton1_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{

}

private: System::Void backgroundWorker1_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
{
	auto t_start = std::chrono::high_resolution_clock::now();
	auto t_end = std::chrono::high_resolution_clock::now();
	auto left_start = std::chrono::high_resolution_clock::now();
	auto left_end = std::chrono::high_resolution_clock::now();
	bool left_down = false;
	bool disabled = false;
	bool m3 = false;
	bool m4 = false;
	bool m5 = false;
	context = interception_create_context();
	interception_set_filter(context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_ALL);

	while (interception_receive(context, device = interception_wait(context), &stroke, 1) > 0)
	{

		InterceptionMouseStroke& mstroke = *(InterceptionMouseStroke*)&stroke;
		t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

		CURSORINFO cursorInfo = { 0 };
		cursorInfo.cbSize = sizeof(cursorInfo);
		GetCursorInfo(&cursorInfo);

		if (mstroke.state & INTERCEPTION_MOUSE_LEFT_BUTTON_UP)
		{
			left_down = false;
		}

		if (mstroke.state & INTERCEPTION_MOUSE_BUTTON_3_DOWN && mouse3->Checked)
		{
			m3 = !m3;
			label4->Text = m3 ? "ON" : "OFF";
			label4->ForeColor = m3 ? ForeColor.Green : ForeColor.Maroon;
		}

		if (mstroke.state & INTERCEPTION_MOUSE_BUTTON_4_DOWN && mouse4->Checked)
		{
			m4 = !m4;
			label4->Text = m4 ? "ON" : "OFF";
			label4->ForeColor = m4 ? ForeColor.Green : ForeColor.Maroon;
		}

		if (mstroke.state & INTERCEPTION_MOUSE_BUTTON_5_DOWN && mouse5->Checked)
		{
			m5 = !m5;
			label4->Text = m5 ? "ON" : "OFF";
			label4->ForeColor = m5 ? ForeColor.Green : ForeColor.Maroon;
		}

		if (m5)
		{
			if ((mstroke.flags == 0)) {
				if (elapsed_time_ms > 7) {
					t_start = std::chrono::high_resolution_clock::now();
					left_start = std::chrono::high_resolution_clock::now();
					if (aim_x != 0 || aim_y != 0) {
						left_end = std::chrono::high_resolution_clock::now();
						double recoil_ms = std::chrono::duration<double, std::milli>(left_end - left_start).count();
						double extra = 38.1 * (screen_height / 1080.0) * (recoil_ms / 1000.0);
						if (!left_down) {
							extra = 0;
						}
						else if (extra > 38.1) {
							extra = 38.1;
						}
						double v_x = double(aim_x) * smooth / 50;
						double v_y = double(aim_y + extra) * smooth / 50;
						if (fabs(v_x) < 1.0) {
							v_x = v_x > 0 ? 1.06 : -1.06;
						}
						if (fabs(v_y) < 1.0) {
							v_y = v_y > 0 ? 1.06 : -1.06;
						}
						mstroke.x += v_x;
						mstroke.y += v_y;
					}
				}
			}
		}
		if (m4)
		{
			if ((mstroke.flags == 0)) {
				if (elapsed_time_ms > 7) {
					t_start = std::chrono::high_resolution_clock::now();
					left_start = std::chrono::high_resolution_clock::now();
					if (aim_x != 0 || aim_y != 0) {
						left_end = std::chrono::high_resolution_clock::now();
						double recoil_ms = std::chrono::duration<double, std::milli>(left_end - left_start).count();
						double extra = 38.1 * (screen_height / 1080.0) * (recoil_ms / 1000.0);
						if (!left_down) {
							extra = 0;
						}
						else if (extra > 38.1) {
							extra = 38.1;
						}
						double v_x = double(aim_x) * smooth / 50;
						double v_y = double(aim_y + extra) * smooth / 50;
						if (fabs(v_x) < 1.0) {
							v_x = v_x > 0 ? 1.06 : -1.06;
						}
						if (fabs(v_y) < 1.0) {
							v_y = v_y > 0 ? 1.06 : -1.06;
						}
						mstroke.x += v_x;
						mstroke.y += v_y;
					}
				}
			}
		}
		if (m3)
		{
			if ((mstroke.flags == 0)) {
				if (elapsed_time_ms > 7) {
					t_start = std::chrono::high_resolution_clock::now();
					left_start = std::chrono::high_resolution_clock::now();
					if (aim_x != 0 || aim_y != 0) {
						left_end = std::chrono::high_resolution_clock::now();
						double recoil_ms = std::chrono::duration<double, std::milli>(left_end - left_start).count();
						double extra = 38.1 * (screen_height / 1080.0) * (recoil_ms / 1000.0);
						if (!left_down) {
							extra = 0;
						}
						else if (extra > 38.1) {
							extra = 38.1;
						}
						double v_x = double(aim_x) * smooth / 50;
						double v_y = double(aim_y + extra) * smooth / 50;
						if (fabs(v_x) < 1.0) {
							v_x = v_x > 0 ? 1.06 : -1.06;
						}
						if (fabs(v_y) < 1.0) {
							v_y = v_y > 0 ? 1.06 : -1.06;
						}
						mstroke.x += v_x;
						mstroke.y += v_y;
					}
				} 
			} 
		}

		interception_send(context, device, &stroke, 1);
	}
}
private: System::Void checkBox2_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{

}
private: System::Void aimmm_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{

}

private: System::Void panel1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{

}
};
}

