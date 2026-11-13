# Complete Example

---

```c++
// Only include you probably need for this to work :)
#include <dxfw11Mini.hpp>

// optional and only active on a debug build
// also: i dont even know if this works... i only tested it with CMake:
// target_compile_definitions(<YOUR_TARGET> <YOUR_CONFIG> DXFW_TRACING)
#define DXFW_TRACING

// optional imgui menu initialization function
// more about it in the README.md
static void InitMenu()
{
    
}

static void RenderMenu()
{
    ImGui::ShowDemoWindow(); // For testing purposes
    
    /* ImGui::Begin("ImMenu", ...);
     * ...
     * ImGui::End();
     */
}

int main()
{
    // Optional! But makes it easier to read your own code
    auto& renderer = dxfw::CRender::Get();
    
    // Optional! Doesn't even need to be called
    renderer.SetMenu_InitFunction(InitMenu);
    
    // Required! But can be a empty void function, like i did with the InitMenu function
    // If not set, your console will tell you
    renderer.SetMenu_RenderFunction(RenderMenu);
    
    // -+-+-+-+-+-+-+- Init Window Function  -+-+-+-+-+-+-+-
    
    const bool init = renderer.InitWindow(
            
            { 1920, 1080 },             // Window Size | Should be set to your full display size if window type is set to Overlay
            
            { 0, 0 },                   // Window Pos  | Set to ( 0 x 0 ) by default if window type is set to Overlay
            
            "WindowTitle",              // Window Title
            
            69,                         // Target FPS | Optional! Default is 60, doesn't do anything because i didn't implement a frame pacing / frame limiting mechanism
            
            dxfw::EWindowType::OVERLAY  // Window Type | Optional! Type: OVERLAY is transparent
                                        // default is type: NORMAL ==> WS_OVERLAPPEDWINDOW aka. normal window
                                       
    );
    
    // -+-+-+-+-+-+-+--+-+-+-+-+-+-+--+-+-+-+-+-+-+--+-+-+-+
    
    // You can just put the InitWindow(...) function into the if. Here it was easier to add comments
    if (!init)
        return -1;
    
    // -+-+-+-+-+-+-+- Main Rendering Loop -+-+-+-+-+-+-+-
    
    // Fully clear for WindowType: OVERLAY
    std::array<FLOAT, 4> clearColor { 0.0F, 0.0F, 0.0F, 0.0F };
    
    // Look at the Functions Documentation if you still dont get it :D
    while (!renderer.WindowShouldClose())
    {
        // Optional Call!
        // Call this before BeginFrame(...)! If you need it...
        // Toggles window click through attribute
        // Toggle Key here is set to [ INSERT ]
        // true == the menu's visibility will also be affected
        renderer.ToggleClickThrough(VK_INSERT, true);
        
        // Take a guess on what this does
        renderer.BeginFrame(clearColor);
        
        // aka. ImGui::GetBackgroundDrawList()
        renderer.GetDrawList(dxfw::EDrawListType::BACK)
            ->AddRectFilled({100.f, 100.f},
                            {200.f, 200.f},
                            ImGui::ColorConvertFloat4ToU32(
                                {1.f, 0.f, 0.f, 1.f}
                            ));
        
        // Opposite of BeginFrame
        // true == ( VSync = ON )
        renderer.EndFrame(true);
    }
    
    return 0;
}
```