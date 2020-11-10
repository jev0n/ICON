#include "MyForm1.h"


using namespace Dll2;

int UIMain()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault;

	MyForm^ NewUi = gcnew MyForm();
	Application::Run(NewUi);

	return 1;
}