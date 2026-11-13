# DXFW11 - Mini
###### est. 2026 - [1MOLY](https://github.com/1MOLY)

---

### Mainly used namespaces:

```c++
namespace dxfw { ... }          // Core Library Part
namespace dxfw::_internal {}    // Internal Part
namespace dxfw::util {}         // Utility Part, can be used if something is needed in your code
```
---

### Features:
- Single Windowing API using D3D11
- Win32 window creation and management
- Direct3D 11 rendering
- Dear ImGui integration
- Transparent overlay windows
- Configurable click-through toggling
- ImGui background, foreground and window draw lists

---

### Usage:
###### [Complete Example](.docs/EXAMPLES.md)

```c++ 
dxfw::CRender::Get()    // singleton getter for the core rendering class

auto& renderer = dxfw::CRender::Get();      // More convenient
if (!renderer.InitWindow(...)) ...
```

```c++
static void InitFunction() { ... }
dxfw::CRender::Get().SetMenu_InitFunction(InitFunction);    // Optional ImGUI Menu initialization
                                                            // Gets called automatically if set
```

```c++
static void RenderFunction() { ... }
dxfw::CRender::Get().SetMenu_RenderFunction(RenderFunction);    // Same usage as SetMenu_InitFunction() but required
                                                                // Can be an emtpy void function
                                                                // Gets called automatically if set
```  

```c++
dxfw::CRender::Get().GetDrawList(       // returns ImDrawlist*
    dxfw::EDrawListType::BACK           // only option for drawing because i was lazy...
);                                      // just use type BACK DrawList and you should be good for normal drawing
```

---

### Compile Definitions:
###### _Can be set through CMake or by #define ... I didn't test the #define method_

- DXFW_TRACING ⇒ only active on debug build if set, prints trace line throughout the DXFW11 code and can be used with the Macro: LOG_POINT;

---