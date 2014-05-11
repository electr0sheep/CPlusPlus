// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
//#include <d3dx11.h>
//#include <d3dx10.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// global declarations
IDXGISwapChain *swapchain;				// the pointer to the swap chain interface
ID3D11Device *dev;						// the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;			// the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;		// the pointer to our back buffer
ID3D11DepthStencilView *zbuffer;		// the pointer to our depth buffer
ID3D11InputLayout *pLayout;				// the pointer to the input layout
ID3D11VertexShader *pVS;				// the pointer to the vertex shader
ID3D11PixelShader *pPS;					// the pointer to the pixel shader
ID3D11Buffer *pVBuffer;					// the pointer to the vertex buffer
ID3D11Buffer *pCBuffer;					// the pointer to the constant buffer

// various buffer structs
struct VERTEX{
	FLOAT X, Y, Z;
	D3DXCOLOR Color;
};
struct PERFRAME{
	D3DXCOLOR Color;
	FLOAT X, Y, Z;
};

// function prototypes
void InitD3D(HWND hWnd);	// sets up and initializes Direct3D
void RenderFrame();			// renders a single frame
void CleanD3D();			// closes Direct3D and releases memory
void InitGraphics();		// creates the shape to render
void InitPipeline();		// loads and prepares the shaders

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	HWND hWnd;
	WNDCLASSEX wc;
	
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our First Direct3D Program", WS_OVERLAPPEDWINDOW, 100, 100, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
	InitD3D(hWnd);

    // enter the main loop:

	MSG msg;

	while(true){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
				break;
		}

        RenderFrame();
	}

	// clean up DirectX and COM
	CleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd){
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;									// one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;					// set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;					// set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// how swap chain is to be used
	scd.OutputWindow = hWnd;								// the window to be used
	scd.SampleDesc.Count = 4;								// how many multisamples
	scd.Windowed = TRUE;									// windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, NULL, &devcon);


	// create the depth buffer texture
	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));

	texd.Width = SCREEN_WIDTH;
	texd.Height = SCREEN_HEIGHT;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 4;
	texd.Format = DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D *pDepthBuffer;
	dev->CreateTexture2D(&texd, NULL, &pDepthBuffer);

	// create the depth buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &zbuffer);
	pDepthBuffer->Release();

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0;	// the closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1;	// the farthest an object can be on the depth buffer is 1.0

	devcon->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
}


// this is the function used to render a single frame
void RenderFrame(){
	D3DXMATRIX matTranslate[4], matRotate[4], matView, matProjection, matFinal[4];

	static float Time = 0.0f; Time += 0.0001f;

	// create a world matrices
	D3DXMatrixRotationY(&matRotate[0], Time);
	D3DXMatrixRotationY(&matRotate[1], Time + FLOAT(D3DXToRadian(180)));
	D3DXMatrixRotationY(&matRotate[2], Time);
	D3DXMatrixRotationY(&matRotate[3], Time + FLOAT(D3DXToRadian(180)));
	D3DXMatrixTranslation(&matTranslate[0], 0.0f, 0.0f, 0.5f);
	D3DXMatrixTranslation(&matTranslate[1], 0.0f, 0.0f, 0.5f);
	D3DXMatrixTranslation(&matTranslate[2], 0.0f, 0.0f, -0.5f);
	D3DXMatrixTranslation(&matTranslate[3], 0.0f, 0.0f, -0.5f);

	// create a view matrix
	D3DXMatrixLookAtLH(&matView,
					   &D3DXVECTOR3(0.0f, 0.0f, 10.0f),		// the camera position
					   &D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// the look-at position
					   &D3DXVECTOR3(0.0f, 1.0f, 0.0f));		// the up direction

    // create a projection matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
							   (FLOAT)D3DXToRadian(45),						// field of view
							   (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT,	// aspect ratio
							   1.0f,										// near view-plane
							   100.0f);										// far view-plane

	// create the final transform
	matFinal[0] = matTranslate[0] * matRotate[0] * matView * matProjection;
	matFinal[1] = matTranslate[1] * matRotate[1] * matView * matProjection;
	matFinal[2] = matTranslate[2] * matRotate[2] * matView * matProjection;
	matFinal[3] = matTranslate[3] * matRotate[3] * matView * matProjection;

	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// clear the depth buffer
	devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// select which primtive type we are using
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal[0], 0, 0);
	devcon->Draw(4, 0);

	// draw each triangle
	/*devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal[0], 0, 0);
	devcon->Draw(3, 0);
	devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal[1], 0, 0);
	devcon->Draw(3, 0);
	devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal[2], 0, 0);
	devcon->Draw(3, 0);
	devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal[3], 0, 0);
	devcon->Draw(3, 0);*/

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(){
	swapchain->SetFullscreenState(FALSE, NULL);		// switch to windowed mode

	// close and release all existing COM objects
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	zbuffer->Release();
	pVBuffer->Release();
	pCBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}


// this is the function that creates the shape to render
void InitGraphics(){
	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] =
	{
		{-1.0f, 1.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{1.0f, 1.0, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{0.0f, -1.0f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)},
		//{1.0f, -1.0f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)}
	};
	

	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;					// write access access by CPU and GPU
	bd.ByteWidth = sizeof(OurVertices);				// size is the OurVertices array
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &pVBuffer);		// create the buffer


	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
}


// this function loads and prepares the shaders
void InitPipeline(){
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 64;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	dev->CreateBuffer(&bd, NULL, &pCBuffer);
	devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
}




/*#include <iostream>
#include <string>

using namespace std;

// Function Prototypes
string infixToPrefix(string, int);

int main(){
	// String variables
	string infixExpression = "";
	string prefixExpression = "";
	// Get input
	while(true){
		cout << "Input an infix boolean expression, to quit, type \"exit\":";
		cin >> infixExpression;
		cout << "You entered: " + infixExpression << endl;
		// Process the expression, or end loop if expression equals exit
		if (infixExpression == "exit")
			break;
		prefixExpression = infixToPrefix(infixExpression, 0);
		cout << "The prefix expression is: " + prefixExpression << endl;
	}
	return 0;
}

string infixToPrefix(string infix, int loc){
	//static string prefix = "";
	if (loc < infix.length()){
		switch (infix[loc]){
		case '+':
			//prefix += infix[loc];
			return infix[loc] + infixToPrefix(infix, loc + 1);
			break;
		case '*':
			//prefix += infix[loc];
			return infix[loc] + infixToPrefix(infix, loc + 1);
			break;
		default:
			//prefix += infix[loc];
			return infix[loc] + infixToPrefix(infix, loc + 1);
			break;
		}
		//prefix += infixToPrefix(infix, loc - 1);
	}
	return "";
}
*/