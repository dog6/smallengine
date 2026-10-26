# Lua Bindings Reference

This document describes the C++ types and functions available to Lua scripts in the SmallzComputing game engine. The bindings use **sol2**.

**Version 4 — Entity control and coordinate corrections**

Changes from version 3:

- `Entman.AddCircle` and `Entman.AddRect` return an entity reference that Lua can retain and use.
- `Entity:GetPosition()` and `Entity:SetPosition(pos)` are documented.
- Position coordinates returned to and supplied by Lua use pixels; the entity methods convert between pixels and Box2D meters.
- The `useMeters` argument is documented for entity creation.
- The Lua `update(dt)` lifecycle callback is documented.
- Known limitations are revised to distinguish implemented functionality from missing features.

---

## 1. Rules for Calling Functions

Lua has two ways to call a function.

| Syntax | Use for | Example |
|---|---|---|
| Colon (`:`) | Object member functions | `v:pixelsToMeters()` |
| Dot (`.`) | Static functions and functions without an object | `Entman.AddRect(...)` |

**Important:** Use a dot for `Entman.AddCircle`, `Entman.AddRect`, `EntityFactory.CreateCircleEntity`, and `EntityFactory.CreateRectEntity`. These functions do not accept an object instance.

Use a dot for all functions in `Keyboard` and `Mouse`. These are function tables, not objects.

For example:

```lua
local position = vec2f(100, 200)

-- Member functions
local meters = position:pixelsToMeters()

-- Static functions
local entity = Entman.AddRect(
    "Player",
    world_id,
    vec2f(100, 200),
    vec2f(50, 50),
    ColorRGB(255, 0, 0),
    true,
    false
)
```

## 2. Registration Order

Register types before registering bindings or assigning globals that use those types.

Recommended order:

1. `RegisterVec2`, `RegisterUVec2`, `RegisterVec2f`
2. `GraphicsLuaBindings::RegisterColor`
3. `RegisterGameWindow`
4. `InputLuaBindings::Register`
5. `RegisterEntity`, `RegisterEntityManager`, `RegisterEntityFactory`
6. `RegisterMathFunctions`
7. Register `b2WorldId` and assign the `world_id` global
8. Assign the `game_window` global after registering `GameWindow`
9. Load and execute the Lua script

The exact order of the global assignments may vary as long as the corresponding types have already been registered.

**Important:** Register a C++ type before assigning a global variable that holds an object of that type. Assigning the object first can result in an empty or incorrect metatable. Later registration does not automatically repair the existing Lua object.

The engine must also register `b2WorldId` separately. The entity creation functions require it as an argument.

## 3. Global Variables

The following globals are expected by the example scripts.

| Name | Type | Description |
|---|---|---|
| `game_window` | `GameWindow*` | Pointer to the active game window |
| `world_id` | `b2WorldId` | Identifier for the physics world |

### `game_window`

Assign a pointer to the existing window. Do not copy the `GameWindow` object.

```cpp
lua["game_window"] = &sg::Game::GetInstance()->GetWindow();
```

The pointer remains valid only while the `Game` object and its window exist.

### `world_id`

Register the Box2D world ID and assign the global:

```cpp
lua.new_usertype<b2WorldId>(
    "b2WorldId",
    sol::no_constructor
);

lua["world_id"] = PhysicsManager::world.worldId;
```

Use the actual world ID belonging to the world in which you want to create entities.

---

## 4. Math Types

### 4.1 `vec2`

A two-dimensional vector with integer coordinates.

**Constructors**

| Call | Result |
|---|---|
| `vec2()` or `vec2.new()` | Vector initialized with default values |
| `vec2(x, y)` or `vec2.new(x, y)` | Vector with integer `x` and `y` |

**Properties**

| Name | Type | Access |
|---|---|---|
| `x` | Integer | Read/write |
| `y` | Integer | Read/write |

**Metamethods**

- `tostring(v)` produces text in the form `vec2(x, y)`.

**Example**

```lua
local v = vec2(3, 4)
v.x = 10

print(v) -- vec2(10, 4)
```

### 4.2 `uvec2`

A two-dimensional vector with unsigned integer coordinates.

**Constructors**

| Call | Result |
|---|---|
| `uvec2()` or `uvec2.new()` | Vector initialized with default values |
| `uvec2(x, y)` or `uvec2.new(x, y)` | Vector with unsigned integer `x` and `y` |

**Properties**

| Name | Type | Access |
|---|---|---|
| `x` | Unsigned integer | Read/write |
| `y` | Unsigned integer | Read/write |

**Metamethods**

- `tostring(v)` produces text in the form `uvec2(x, y)`.

Unsigned coordinates cannot represent negative values.

**Example**

```lua
local resolution = uvec2(1280, 720)
print(resolution) -- uvec2(1280, 720)
```

### 4.3 `vec2f`

A two-dimensional vector with floating-point coordinates.

Use `vec2f` for positions, sizes, movement distances, and other values requiring fractional precision.

**Constructors**

| Call | Result |
|---|---|
| `vec2f()` or `vec2f.new()` | Vector initialized with default values |
| `vec2f(x, y)` or `vec2f.new(x, y)` | Vector with floating-point `x` and `y` |

**Properties**

| Name | Type | Access |
|---|---|---|
| `x` | Float | Read/write |
| `y` | Float | Read/write |

**Functions**

| Function | Returns | Description |
|---|---|---|
| `v:pixelsToMeters()` | `vec2f` | Returns a copy converted from pixels to meters |
| `v:metersToPixels()` | `vec2f` | Returns a copy converted from meters to pixels |

These functions do not modify the original vector.

The conversion factor is `smath.PIXELS_PER_METER`.

**Operators**

| Operator | Operands | Result |
|---|---|---|
| `a + b` | Two `vec2f` values | `vec2f` |
| `a - b` | Two `vec2f` values | `vec2f` |
| `a * s` | `vec2f` and number | `vec2f` |
| `a == b` | Two `vec2f` values | Boolean |
| `tostring(v)` | `vec2f` | Text in the form `vec2f(x, y)` |

Multiplication requires the vector on the left:

```lua
local a = vec2f(100, 50)
local b = vec2f(20, 10)

local sum = a + b       -- vec2f(120, 60)
local half = sum * 0.5  -- vec2f(60, 30)

local meters = a:pixelsToMeters()

print(a == b) -- false
```

The Lua binding does not currently expose vector division, unary negation, or a length function. Although the C++ type supports additional operators, they are not necessarily available through Lua.

### 4.4 `smath`

A table of math functions and constants.

| Name | Type | Description |
|---|---|---|
| `smath.random_int(...)` | Function | Generates a random integer |
| `smath.random_float(...)` | Function | Generates a random floating-point value |
| `smath.PIXELS_PER_METER` | Number | Pixels-per-meter conversion factor |

The random functions use the corresponding C++ functions. Consult their C++ declarations for the supported arguments.

`smath.PIXELS_PER_METER` is a copy of the value at registration time. If the underlying C++ constant changes afterward, the Lua value does not update automatically.

**Example**

```lua
local die_roll = smath.random_int(1, 6)
local position = vec2f(400, 300):pixelsToMeters()
```

---

## 5. Graphics Types

### 5.1 `ColorRGB`

Represents a color with red, green, and blue channels.

**Constructors**

```lua
local red = ColorRGB(255, 0, 0)
local blue = ColorRGB.new(0, 0, 255)
```

The constructor accepts integer channel values and uses `ClampByte` to constrain each channel to the range 0–255.

**Properties**

| Lua property | C++ member | Access |
|---|---|---|
| `r` | `Red` | Read/write |
| `g` | `Green` | Read/write |
| `b` | `Blue` | Read/write |

**Example**

```lua
local color = ColorRGB(255, 0, 0)
color.g = 128
```

### 5.2 `ColorRGBA`

Represents a color with red, green, blue, and alpha channels.

**Constructors**

```lua
local glass = ColorRGBA(0, 120, 255, 100)
local opaque_red = ColorRGBA.new(255, 0, 0, 255)
```

Channel values are constrained to 0–255.

**Properties**

| Lua property | C++ member | Access |
|---|---|---|
| `r` | `Red` | Read/write |
| `g` | `Green` | Read/write |
| `b` | `Blue` | Read/write |
| `a` | `Alpha` | Read/write |

The color bindings do not currently expose a `tostring` metamethod.

---

## 6. Core Types

### 6.1 `GameWindow`

Represents the game window and provides access to selected window controls.

Lua cannot construct or copy a `GameWindow`. Use the global `game_window`.

**Properties**

| Property | Type | Access |
|---|---|---|
| `resolution` | `uvec2` | Read-only |

**Functions**

Call these functions using colon syntax.

| Function | Arguments | Returns | Description |
|---|---|---|---|
| `is_open()` | None | Boolean | Checks whether the window is open |
| `close()` | None | Nothing | Closes the window |
| `set_title(title)` | String | Nothing | Sets the window title |
| `set_vsync(enabled)` | Boolean | Nothing | Enables or disables vertical synchronization |
| `set_framerate_limit(limit)` | Unsigned integer | Nothing | Sets the frame-rate limit; `0` means unlimited |

**Example**

```lua
game_window:set_title("My Game")
game_window:set_vsync(true)
game_window:set_framerate_limit(60)

print(game_window.resolution)
```

The `Render` function and the underlying `window` and `renderer` members are not exposed through these bindings.

The global pointer must not be used after its associated window has been destroyed.

---

## 7. Input Types

Input bindings are provided by `InputLuaBindings`. They query the current state of the keyboard and mouse; they do not provide event callbacks.

### 7.1 `Key`

A read-only table of SFML 3 keyboard key names. Use the named constants rather than numeric values.

| Group | Names |
|---|---|
| Letters | `A` through `Z` |
| Digits | `Num0` through `Num9` |
| Numeric keypad | `Numpad0` through `Numpad9`, `Add`, `Subtract`, `Multiply`, `Divide` |
| Function keys | `F1` through `F15` |
| Arrows | `Left`, `Right`, `Up`, `Down` |
| Editing | `Escape`, `Space`, `Enter`, `Backspace`, `Tab`, `Insert`, `Delete`, `Home`, `End`, `PageUp`, `PageDown`, `Pause`, `Menu` |
| Modifiers | `LControl`, `RControl`, `LShift`, `RShift`, `LAlt`, `RAlt`, `LSystem`, `RSystem` |
| Symbols | `LBracket`, `RBracket`, `Semicolon`, `Comma`, `Period`, `Apostrophe`, `Slash`, `Backslash`, `Grave`, `Equal`, `Hyphen` |

These names follow SFML 3 conventions. SFML 2 uses different names for some keys.

**Example**

```lua
local jump_key = Key.Space
```

### 7.2 `MouseButton`

A read-only table of mouse button names.

| Name | Description |
|---|---|
| `Left` | Left mouse button |
| `Right` | Right mouse button |
| `Middle` | Middle mouse button |
| `Extra1` | First extra mouse button |
| `Extra2` | Second extra mouse button |

### 7.3 `Keyboard`

A table of keyboard functions. Call all functions using dot syntax.

| Function | Arguments | Returns | Description |
|---|---|---|---|
| `Keyboard.IsDown(key)` | One key | Boolean | True while the key is held |
| `Keyboard.IsUp(key)` | One key | Boolean | True while the key is not held |
| `Keyboard.AnyDown(key, ...)` | One or more keys | Boolean | True if at least one key is held |
| `Keyboard.AllDown(key, ...)` | One or more keys | Boolean | True if all specified keys are held |
| `Keyboard.Axis(negative, positive, scale)` | Two keys and optional number | Number | Returns a signed directional value |

The default axis scale is `1`.

| Key state | Result |
|---|---:|
| Only the positive key is held | `scale` |
| Only the negative key is held | `-scale` |
| Both keys are held | `0` |
| Neither key is held | `0` |

**Example**

```lua
if Keyboard.IsDown(Key.Space) then
    print("Jump")
end

local speed = 300
local vertical = Keyboard.Axis(Key.S, Key.W, speed)

local fast = Keyboard.AnyDown(Key.LShift, Key.RShift)
local save = Keyboard.AllDown(Key.LControl, Key.S)
```

### 7.4 `Mouse`

A table of mouse functions. Call all functions using dot syntax.

| Function | Arguments | Returns | Description |
|---|---|---|---|
| `Mouse.IsDown(button)` | One button | Boolean | True while the button is held |
| `Mouse.IsUp(button)` | One button | Boolean | True while the button is not held |
| `Mouse.AnyDown(button, ...)` | One or more buttons | Boolean | True if any specified button is held |
| `Mouse.AllDown(button, ...)` | One or more buttons | Boolean | True if all specified buttons are held |
| `Mouse.Axis(negative, positive, scale)` | Two buttons and optional number | Number | Returns a signed directional value |
| `Mouse.Position()` | None | `vec2f` | Cursor position in window pixels |
| `Mouse.ScaledPosition(scale)` | `vec2f` | `vec2f` | Cursor position multiplied by per-axis scale factors |

The mouse functions use the game window automatically; no window argument is required.

The cursor can leave the window, so its coordinates can be negative or greater than the window dimensions.

**Example**

```lua
if Mouse.IsDown(MouseButton.Left) then
    local cursor = Mouse.Position()
    print(cursor)
end

local half = Mouse.ScaledPosition(vec2f(0.5, 0.5))
```

---

## 8. Entity Types

### 8.1 `Entity`

An entity is a game object that holds components.

**Functions**

Call member functions using colon syntax.

| Function | Arguments | Returns | Description |
|---|---|---|---|
| `entity:AddComponent(name)` | Component type name | Component wrapper | Adds a component and returns its wrapper |
| `entity:GetComponent(name)` | Component type name | Component wrapper | Retrieves the requested component |
| `entity:GetPosition()` | None | `vec2f` | Returns the entity's position in pixels |
| `entity:SetPosition(pos)` | `vec2f` | Nothing | Sets the entity's position using pixel coordinates |

The available component wrapper methods depend on the components registered in C++.

The `name` property is not currently registered through this binding.

#### Position and coordinate conventions

Lua should use **pixels** when reading and setting entity positions through `GetPosition()` and `SetPosition()`.

```lua
local position = entity:GetPosition()
position.x = position.x + 100

entity:SetPosition(position)
```

The C++ entity methods convert between pixels and Box2D meters using the engine's `PIXELS_PER_METER` scale.

Conceptually:

\[
\text{meters}=\frac{\text{pixels}}{\text{PIXELS\_PER\_METER}}
\]

\[
\text{pixels}=\text{meters}\times\text{PIXELS\_PER\_METER}
\]

Keep the scale consistent across entity creation, position access, and rendering. Do not pass pixel coordinates directly to Box2D as if they were meters.

**Important:** Box2D body positions normally represent the body's center. The entity's drawing origin and the factory's position convention must be considered when placing rectangles and circles.

`SetPosition()` updates the Box2D transform. It does not set velocity or simulate movement over time. Calling it every frame teleports the body to the requested position.

Moving a static body this way can be useful for simple demonstrations, but static bodies are intended to remain stationary. For continuously moving physics objects, consider a kinematic or dynamic body.

### 8.2 `Entman` (`EntityManager`)

Provides entity creation functions through the globally registered `Entman` usertype. Lua cannot construct an `EntityManager` through this binding.

**Functions**

| Function | Call style | Returns | Description |
|---|---|---|---|
| `Entman.MakeEntity(...)` | Member / colon | Depends on C++ declaration | Creates an entity through the manager |
| `Entman.RemoveEntity(...)` | Member / colon | Depends on C++ declaration | Removes an entity |
| `Entman.AddCircle(...)` | Static / dot | `Entity*` | Creates and registers a circle entity |
| `Entman.AddRect(...)` | Static / dot | `Entity*` | Creates and registers a rectangle entity |

`MakeEntity` and `RemoveEntity` are member functions and require an actual manager instance. The global `Entman` type does not, by itself, provide an instance for these methods.

#### `AddCircle` arguments

| Position | Argument | Type |
|---:|---|---|
| 1 | `name` | String |
| 2 | `w` | `b2WorldId` |
| 3 | `pos` | `vec2f` |
| 4 | `radius` | Float |
| 5 | `col` | `ColorRGB` or `ColorRGBA` |
| 6 | `isStatic` | Boolean |
| 7 | `useMeters` | Boolean |

#### `AddRect` arguments

| Position | Argument | Type |
|---:|---|---|
| 1 | `name` | String |
| 2 | `w` | `b2WorldId` |
| 3 | `pos` | `vec2f` |
| 4 | `size` | `vec2f` |
| 5 | `col` | `ColorRGB` or `ColorRGBA` |
| 6 | `isStatic` | Boolean |
| 7 | `useMeters` | Boolean |

The color argument accepts either `ColorRGB` or `ColorRGBA`.

**Position and size units:** The `useMeters` argument controls the units expected by the C++ entity factory. When `false`, creation uses pixel-based values; when `true`, values are supplied in meters. Keep this consistent with the factory implementation and the position conversion used by the entity.

The `isStatic` argument determines whether the created physics body is static.

**Example: create and retain a static rectangle**

```lua
local rectangle = Entman.AddRect(
    "Player",
    world_id,
    vec2f(400, 300),
    vec2f(50, 50),
    ColorRGB(255, 0, 0),
    true,   -- static body
    false   -- pixel-based creation coordinates
)

if rectangle then
    local position = rectangle:GetPosition()
    print(position)
end
```

The returned pointer refers to the entity owned by the entity manager. Keep the entity manager alive while using the pointer, and do not use the Lua reference after the entity has been removed or destroyed.

### 8.3 `EntityFactory`

Provides functions for creating entities without automatically adding them to the entity manager.

Lua cannot construct an `EntityFactory` object. Call its functions using dot syntax.

| Function | Arguments | Returns |
|---|---|---|
| `EntityFactory.CreateCircleEntity(...)` | Same arguments as `AddCircle` | Entity result defined by the C++ binding |
| `EntityFactory.CreateRectEntity(...)` | Same arguments as `AddRect` | Entity result defined by the C++ binding |

Both functions accept `ColorRGB` or `ColorRGBA`.

**Example**

```lua
local coin = EntityFactory.CreateCircleEntity(
    "Coin",
    world_id,
    vec2f(100, 100),
    20,
    ColorRGBA(255, 215, 0, 255),
    false,
    false
)
```

**Ownership warning:** The exact ownership behavior depends on the registered C++ return type. If the factory returns `std::unique_ptr<Entity>`, Lua must not duplicate ownership of that entity or use it in conflicting ownership contexts. Verify the actual sol2 binding before retaining or transferring the result.

---

## 9. Lua Script Lifecycle

The engine's Lua update loop looks for a global function named **`update`**, using a lowercase `u`, and calls it with the frame's delta time.

```lua
function update(dt)
    -- Runs each time the engine invokes LuaManager::Update().
    -- dt is elapsed time in seconds.
end
```

The engine does not automatically call a function named `Init`. A script can call its own initialization function at the end of the file, after defining it.

**Example**

```lua
local rectangle = nil

local function Init()
    rectangle = Entman.AddRect(
        "DemoRectangle",
        world_id,
        vec2f(100, 100),
        vec2f(50, 50),
        ColorRGB(255, 0, 0),
        true,
        false
    )
end

function update(dt)
    if not rectangle then
        return
    end

    local position = rectangle:GetPosition()
    position.x = position.x + 100 * dt
    rectangle:SetPosition(position)
end

Init()
```

This example moves the rectangle by setting its position each frame. It assumes the entity creation, position conversion, and renderer use the same pixels-per-meter scale.

If the engine loads multiple scripts, each script's lifecycle and entity references should be managed according to the engine's script ownership and update implementation.

---

## 10. Complete Examples

### 10.1 Create the ground and walls

This example creates a floor and two walls using the active window's resolution.

```lua
local WALL_THICKNESS = 32.0
local STATIC_BODY = true
local USE_METERS = false
local WHITE = ColorRGB(255, 255, 255)

local res_x = game_window.resolution.x
local res_y = game_window.resolution.y

local ground_size = vec2f(res_x, 32.0)
local ground_pos = vec2f(
    ground_size.x * 0.5,
    res_y - ground_size.y * 0.5
)

Entman.AddRect(
    "Ground",
    world_id,
    ground_pos,
    ground_size,
    WHITE,
    STATIC_BODY,
    USE_METERS
)

local wall_size = vec2f(WALL_THICKNESS, res_y)

Entman.AddRect(
    "LeftWall",
    world_id,
    vec2f(WALL_THICKNESS * 0.5, res_y * 0.5),
    wall_size,
    WHITE,
    STATIC_BODY,
    USE_METERS
)

Entman.AddRect(
    "RightWall",
    world_id,
    vec2f(res_x - WALL_THICKNESS * 0.5, res_y * 0.5),
    wall_size,
    WHITE,
    STATIC_BODY,
    USE_METERS
)
```

### 10.2 Read input

This example reads keyboard and mouse state from inside the engine's update callback.

```lua
local PLAYER_SPEED = 300

function update(dt)
    local vertical = Keyboard.Axis(
        Key.S,
        Key.W,
        PLAYER_SPEED
    )

    local horizontal = Keyboard.Axis(
        Key.A,
        Key.D,
        PLAYER_SPEED
    )

    local quit = Keyboard.IsDown(Key.Escape)
    local serve = Keyboard.AnyDown(Key.Space, Key.Enter)
    local click = Mouse.IsDown(MouseButton.Left)

    -- Use these values in your game logic.
    -- This example intentionally does not move an entity.
end
```

The input functions report current key and button states. They do not independently detect whether a key was newly pressed during the current frame.

### 10.3 Move a static rectangle around the screen

This example creates a static rectangle and moves it clockwise around the screen using position updates.

```lua
local WIDTH = game_window.resolution.x
local HEIGHT = game_window.resolution.y

local RECT_WIDTH = 50
local RECT_HEIGHT = 50

local LEFT = RECT_WIDTH * 0.5
local RIGHT = WIDTH - RECT_WIDTH * 0.5
local TOP = RECT_HEIGHT * 0.5
local BOTTOM = HEIGHT - RECT_HEIGHT * 0.5

local SPEED = 200
local edge = 1
local rectangle = nil

local function Init()
    rectangle = Entman.AddRect(
        "MovingStaticRectangle",
        world_id,
        vec2f(WIDTH * 0.5, HEIGHT * 0.5),
        vec2f(RECT_WIDTH, RECT_HEIGHT),
        ColorRGB(255, 0, 0),
        true,
        false
    )
end

function update(dt)
    if not rectangle then
        return
    end

    local pos = rectangle:GetPosition()
    local distance = SPEED * dt

    if edge == 1 then
        pos.x = math.min(pos.x + distance, RIGHT)
        if pos.x >= RIGHT then
            edge = 2
        end
    elseif edge == 2 then
        pos.y = math.min(pos.y + distance, BOTTOM)
        if pos.y >= BOTTOM then
            edge = 3
        end
    elseif edge == 3 then
        pos.x = math.max(pos.x - distance, LEFT)
        if pos.x <= LEFT then
            edge = 4
        end
    else
        pos.y = math.max(pos.y - distance, TOP)
        if pos.y <= TOP then
            edge = 1
        end
    end

    rectangle:SetPosition(pos)
end

Init()
```

This example assumes that `GetPosition()` and `SetPosition()` use pixel coordinates and that the rectangle's body position corresponds to its center. It also assumes that the factory and renderer use the same conversion scale.

For an object that should move naturally and interact with physics, use an appropriate moving body type instead of repeatedly teleporting a static body.

---

## 11. Known Limitations

- `Entman` does not provide Lua with a manager instance for `MakeEntity` and `RemoveEntity`; expose an instance if scripts need these methods.
- `b2WorldId` must be registered in C++, and `world_id` must be assigned before scripts use it.
- `vec2`, `uvec2`, and the color types do not expose the same operators as `vec2f`.
- `vec2f` does not expose division, unary negation, or a length function through Lua.
- `GameWindow` does not currently expose a general-purpose window-size function beyond its read-only `resolution` property.
- `Mouse.ScaledPosition(scale)` requires the caller to provide a scale vector. The cursor can be outside the window.
- Input queries read current states. There is no built-in just-pressed or just-released query; scripts must track previous states to detect transitions.
- The Lua update callback is named `update(dt)` and is called by the engine's update path. It is not a standalone Lua game loop.
- `Entity:GetPosition()` and `Entity:SetPosition(pos)` operate in pixels and convert to or from Box2D meters. Other APIs may have different unit conventions; check the relevant C++ implementation.
- `SetPosition()` updates the body's transform. It does not set velocity or provide continuous physics-driven movement.
- Static bodies are intended to remain stationary. Repeatedly changing their transforms is suitable for simple demonstrations but is generally not the preferred way to implement moving physics objects.
- `Entman.AddRect` and `Entman.AddCircle` return pointers to entities managed by the entity manager. Scripts must not retain or use references after those entities are destroyed.
- The exact ownership and lifetime behavior of `EntityFactory` results depends on the C++ return type and sol2 binding.
- `ColorRGB` and `ColorRGBA` do not expose a `tostring` metamethod.
- Unsigned `uvec2` values cannot represent negative coordinates.
- The `smath.PIXELS_PER_METER` Lua value is a copy of the C++ value at registration time.
- The `game_window` pointer is only valid while the associated game and window exist.
- The available component names and methods depend on which component wrappers are registered in C++.
