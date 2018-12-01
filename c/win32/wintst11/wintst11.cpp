// Include the defaut Win32 header, you need functions from it
#include <windows.h>
 
// Declare a HWND (Window Handle) for every control you want to use in your application.
// In this tutorial, a button and a text field are used. The window itself also has a window handle.
HWND button, hwnd, editHwnd;
 
// This method is the Window Procedure. The window procedure handles messages sent to the window. A message
// can be sent from another application or from the OS itself to initiate an action in the application.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// This switch block differentiates between the message type that could have been received. If you want to
	// handle a specific type of message in your application, just define it in this block.
    switch(msg)
    {
		// This message type is used by the OS to close a window. Just closes the window using DestroyWindow(hwnd);
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
		// This message type is part of the WM_CLOSE case. After the DestroyWindow(hwnd) function is called, a 
		// WM_DESTROY message is sent to the window, which actually closes it.
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
		// This message type is an important one for GUI programming. It symbolizes an event for a button for example.
		case WM_COMMAND:
			// To differentiate between controls, compare the HWND of, for example, the button to the HWND that is passed
			// into the LPARAM parameter. This way you can establish control-specific actions.
			if (lParam == (LPARAM)button && wParam == BN_CLICKED)
			{
				// The button was clicked, this is your proof.
				MessageBoxA(hwnd, "Button is pressed!", "test", MB_ICONINFORMATION);
			}
		break;
        default:
			// When no message type is handled in your application, return the default window procedure. In this case the message
			// will be handled elsewhere or not handled at all.
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
 
    return 0;
}
 
// This function is the entrypoint of your application. Consider it the main function. The code in this function will be executed
// at first.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// In order to be able to create a window you need to have a window class available. A window class can be created for your
	// application by registering one. The following struct declaration and fill provides details for a new window class.
    WNDCLASSEX wc;
 
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(hInstance, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = LPCSTR("GijSoft");
    wc.hIconSm       = NULL;
 
	// This function actually registers the window class. If the information specified in the 'wc' struct is correct,
	// the window class should be created and no error is returned.
    if(!RegisterClassEx(&wc))
    {
        return 0;
    }
 
    // This function creates the first window. It uses the window class registered in the first part, and takes a title,
	// style and position/size parameters. For more information about style-specific definitions, refer to the MSDN where
	// extended documentation is available.
    hwnd = CreateWindowExA(WS_EX_CLIENTEDGE, "GijSoft", "Win32 C Window application by evolution536",
        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
        CW_USEDEFAULT, CW_USEDEFAULT, 320, 125, NULL, NULL, hInstance, NULL);
 
	// This function creates the button that is displayed on the window. It takes almost the same parameter types as the function
	// that created the window. A thing to note here though, is BS_DEFPUSHBUTTON, and BUTTON as window class, which is an existing one.
	button = CreateWindowA("BUTTON", "Random Button", (WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON)
		, 10, 10, 140, 30, hwnd, NULL, hInstance, NULL);
 
	// This function creates the text field that is displayed on the window. It is almost the same as the function that created the
	// button. Note the EDIT as window class, which is an existing window class defining a "text field".
	editHwnd = CreateWindowA("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 50, 280, 25, hwnd, NULL, hInstance, NULL); 
 
	// In Win32 you need SendMessage for a lot of GUI functionality and altering. The purpose of this function is very wide and not
	// explainable in this tutorial. Refer to MSDN for specific information about a use of this function. In this case it is used to
	// set a text value into the text field created on the window.
	SendMessage(editHwnd, WM_SETTEXT, NULL, (LPARAM)L"Some random text");
 
	// This block checks the integrity of the created window and it's controls. If a control did not succeed creation, the window
	// is not created succesfully, hence it should not be shown.
    if(!hwnd || !button || !editHwnd)
    {
        return 0;
    }
 
	// Everything went right, show the window including all controls.
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
 
	// This part is the "message loop". This loop ensures the application keeps running and makes the window able to receive messages
	// in the WndProc function. You must have this piece of code in your GUI application if you want it to run properly.
	MSG Msg;
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {		
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
 
    return 0;
}