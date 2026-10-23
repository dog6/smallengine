# Lua Bindings Reference

This document describes the C++ types and functions that Lua scripts can use.
The bindings use the sol2 library.

**Version 2.** Changes from version 1:

- `vec2`, `uvec2`, and `vec2f` now have a call constructor. You can write `vec2f(x, y)` or `vec2f.new(x, y)`.
- The global `game_window` is now documented.
- The registration order and the list of known limits are updated.

## 1. Rules for Calling Functions

Lua has two ways to call a function.

| Syntax | Use for | Example |
|--------|---------|---------|
| Colon (`:`) | Functions that work on an object (member functions) | `v:pixelsToMeters()` |
| Dot (`.`) | Functions that do not use an object (static functions and factories) | `ColorRGB.new(1, 2, 3)` |

**WARNING**
Use a dot for `Entman.AddCircle`, `Entman.AddRect`, `EntityFactory.CreateCircleEntity`, and `EntityFactory.CreateRectEntity`. These functions do not accept an object. A colon call gives an argument type error.

## 2. Registration Order

Call the register functions in this order. A type must exist before another binding uses it.

1. `RegisterVec2`, `RegisterUVec2`, `RegisterVec2f`
2. `GraphicsLuaBindings::RegisterColor`
3. `RegisterGameWindow` (uses `uvec2`)
4. `RegisterEntity`, `RegisterEntityManager`, `RegisterEntityFactory` (use `vec2f`, `ColorRGB`, `ColorRGBA`, and `b2WorldId`)
5. `RegisterMathFunctions`
6. Set the global variables (see section 3)
7. Load the script

**WARNING**
Set a global variable that holds a C++ object only after the type of that object is registered. If you set the variable first, sol2 gives the object an empty metatable. A later registration does not change it. The script then gives the error `attempt to index a sol.GameWindow* value`.

**NOTE**
The code does not register `b2WorldId`. Register it in a different place. If you do not, the functions that need a world argument cannot be called from Lua.

## 3. Global Variables

C++ sets these variables before it loads a script.

| Name | Type | Set in | Description |
|------|------|--------|-------------|
| `game_window` | `GameWindow` (pointer) | `RegisterGameWindow` | The game window. |
| `world_id` | `b2WorldId` | C++ code that you write | The physics world. Register `b2WorldId` first. |

**C++ code for `game_window`**

Assign a pointer. Do not assign a reference. `GameWindow` cannot be copied.

```cpp
lua["game_window"] = &sg::Game::GetInstance()->GetWindow();
```

**C++ code for `world_id`**

```cpp
lua.new_usertype<b2WorldId>("b2WorldId", sol::no_constructor);
lua["world_id"] = PhysicsManager::world.worldId;
```

---

## 4. Math Types

### 4.1 vec2

A 2D vector with integer values.

**Constructors**

Both call styles give the same result.

| Call | Result |
|------|--------|
| `vec2()` or `vec2.new()` | Vector with default values |
| `vec2(x, y)` or `vec2.new(x, y)` | Vector with integer `x` and `y` |

**Properties** (read and write)

| Name | Type |
|------|------|
| `x` | integer |
| `y` | integer |

**Metamethods**

- `tostring(v)` gives the text `vec2(x, y)`.

**Example**

```lua
local v = vec2(3, 4)
v.x = 10
print(v)  -- vec2(10, 4)
```

### 4.2 uvec2

A 2D vector with unsigned integer values.

**Constructors**

Both call styles give the same result.

| Call | Result |
|------|--------|
| `uvec2()` or `uvec2.new()` | Vector with default values |
| `uvec2(x, y)` or `uvec2.new(x, y)` | Vector with unsigned integer `x` and `y` |

**Properties** (read and write)

| Name | Type |
|------|------|
| `x` | unsigned integer |
| `y` | unsigned integer |

**Metamethods**

- `tostring(v)` gives the text `uvec2(x, y)`.

**Example**

```lua
local size = uvec2(1280, 720)
print(size)  -- uvec2(1280, 720)
```

### 4.3 vec2f

A 2D vector with floating-point values. Use this type for positions and sizes in the physics world.

**Constructors**

Both call styles give the same result.

| Call | Result |
|------|--------|
| `vec2f()` or `vec2f.new()` | Vector with default values |
| `vec2f(x, y)` or `vec2f.new(x, y)` | Vector with float `x` and `y` |

**Properties** (read and write)

| Name | Type |
|------|------|
| `x` | float |
| `y` | float |

**Functions**

| Function | Returns | Description |
|----------|---------|-------------|
| `v:pixelsToMeters()` | `vec2f` | Divides `x` and `y` by `PIXELS_PER_METER`. Does not change `v`. |
| `v:metersToPixels()` | `vec2f` | Multiplies `x` and `y` by `PIXELS_PER_METER`. Does not change `v`. |

**Operators**

| Operator | Operands | Result |
|----------|----------|--------|
| `a + b` | `vec2f`, `vec2f` | `vec2f` |
| `a - b` | `vec2f`, `vec2f` | `vec2f` |
| `a * s` | `vec2f`, number | `vec2f` |
| `a == b` | `vec2f`, `vec2f` | boolean |
| `tostring(v)` | `vec2f` | text `vec2f(x, y)` |

**NOTE**
The multiplication operator works only with the vector on the left side. `2 * v` gives an error. Write `v * 2`.

There is no division operator, no negation operator, and no length function.

**Example**

```lua
local a = vec2f(100, 50)
local b = vec2f.new(20, 10)
local sum = a + b          -- vec2f(120, 70)
local half = sum * 0.5     -- vec2f(60, 35)
local m = a:pixelsToMeters()
print(a == b)              -- false
```

### 4.4 smath

A table of math values and functions.

| Name | Type | Description |
|------|------|-------------|
| `smath.random_int(...)` | function | Gives a random integer. |
| `smath.random_float(...)` | function | Gives a random float. |
| `smath.PIXELS_PER_METER` | number | The number of pixels in one meter. |

**NOTE**
The arguments of `random_int` and `random_float` come from `sg::Core::random_int` and `sg::Core::random_float`. Look at the C++ declarations for the argument list. A common form is `(min, max)`.

`smath.PIXELS_PER_METER` is a copy of the value at the time of registration. If the C++ value changes later, the Lua value does not change.

**Example**

```lua
local n = smath.random_int(1, 6)
local pos = vec2f(400, 300):pixelsToMeters()
```

---

## 5. Graphics Types

### 5.1 ColorRGB

A color with red, green, and blue values.

**Constructors**

Both calls give the same result.

| Call | Result |
|------|--------|
| `ColorRGB(r, g, b)` | New color |
| `ColorRGB.new(r, g, b)` | New color |

The constructor accepts integers. It uses `ClampByte` to limit each value. The expected range is 0 to 255.

**Properties** (read and write)

| Name | C++ member |
|------|-----------|
| `r` | `Red` |
| `g` | `Green` |
| `b` | `Blue` |

**Example**

```lua
local red = ColorRGB(255, 0, 0)
red.g = 128
```

### 5.2 ColorRGBA

A color with red, green, blue, and alpha (opacity) values.

**Constructors**

| Call | Result |
|------|--------|
| `ColorRGBA(r, g, b, a)` | New color |
| `ColorRGBA.new(r, g, b, a)` | New color |

The constructor uses `ClampByte` to limit each value.

**Properties** (read and write)

| Name | C++ member |
|------|-----------|
| `r` | `Red` |
| `g` | `Green` |
| `b` | `Blue` |
| `a` | `Alpha` |

**Example**

```lua
local glass = ColorRGBA(0, 120, 255, 100)
```

---

## 6. Core Types

### 6.1 GameWindow

The game window. It holds the SFML window and the renderer.

**WARNING**
Lua cannot create a `GameWindow`. The type has no Lua constructor. Lua cannot copy the object. Use the global `game_window` (see section 3).

**Properties**

| Name | Type | Access |
|------|------|--------|
| `resolution` | `uvec2` | Read only |

**Functions** (call with a colon)

| Function | Arguments | Returns | Description |
|----------|-----------|---------|-------------|
| `is_open()` | none | boolean | Tells if the window is open. |
| `close()` | none | nothing | Closes the window. |
| `set_title(title)` | text | nothing | Sets the window title. |
| `set_vsync(enabled)` | boolean | nothing | Turns vertical sync on or off. |
| `set_framerate_limit(limit)` | unsigned integer | nothing | Sets the maximum frame rate. Use 0 for no limit. |

**Example**

```lua
game_window:set_title("My Game")
game_window:set_vsync(true)
game_window:set_framerate_limit(60)
print(game_window.resolution)
```

**NOTE**
The `Render` function and the `window` and `renderer` members are not available in Lua.

The pointer in `game_window` is valid only while the `Game` object and its window exist. Do not use it after the window is destroyed.

---

## 7. Entity Types

### 7.1 Entity

A game object. An entity holds components.

**Functions** (call with a colon)

| Function | Arguments | Returns | Description |
|----------|-----------|---------|-------------|
| `AddComponent(name)` | text | component wrapper | Adds a component of type `name`. Returns the new component. |
| `GetComponent(name)` | text | component wrapper | Gets the component of type `name`. |

The functions use `WrapComponent` to make the Lua object. The properties of the returned object depend on the component type.

**NOTE**
The `name` property is not registered. The line is a comment in the code.

**Example**

```lua
local comp = entity:GetComponent("Transform")
```

The component names that are valid depend on the C++ component list.

### 7.2 Entman (EntityManager)

The entity manager. Lua cannot create this type.

**Functions**

| Function | Call style | Description |
|----------|------------|-------------|
| `MakeEntity` | member (colon) | Makes an entity. The arguments come from `EntityManager::MakeEntity`. |
| `RemoveEntity` | member (colon) | Removes an entity. The arguments come from `EntityManager::RemoveEntity`. |
| `AddCircle(...)` | static (dot) | Makes a circle entity. Adds it to the global entity manager. |
| `AddRect(...)` | static (dot) | Makes a rectangle entity. Adds it to the global entity manager. |

**NOTE**
`MakeEntity` and `RemoveEntity` need an `EntityManager` object. The code does not give Lua a variable for the manager. Make one in C++ if the scripts need these functions.

**AddCircle arguments**

| Position | Name | Type |
|----------|------|------|
| 1 | `name` | text |
| 2 | `w` | `b2WorldId` |
| 3 | `pos` | `vec2f` |
| 4 | `radius` | float |
| 5 | `col` | `ColorRGB` or `ColorRGBA` |
| 6 | `isStatic` | boolean |
| 7 | `useMeters` | boolean |

**AddRect arguments**

| Position | Name | Type |
|----------|------|------|
| 1 | `name` | text |
| 2 | `w` | `b2WorldId` |
| 3 | `pos` | `vec2f` |
| 4 | `size` | `vec2f` |
| 5 | `col` | `ColorRGB` or `ColorRGBA` |
| 6 | `isStatic` | boolean |
| 7 | `useMeters` | boolean |

Neither function returns a value.

**Example**

```lua
Entman.AddCircle("ball", world_id, vec2f(400, 100), 20,
                 ColorRGB(255, 0, 0), false, false)

Entman.AddRect("floor", world_id, vec2f(400, 580), vec2f(800, 40),
               ColorRGBA(80, 80, 80, 255), true, false)
```

### 7.3 EntityFactory

A set of functions that make entities. These functions do not add the entity to the entity manager.

Lua cannot create this type. Use a dot to call the functions.

**Functions**

| Function | Arguments | Returns |
|----------|-----------|---------|
| `EntityFactory.CreateCircleEntity(...)` | Same as `AddCircle` | The new entity |
| `EntityFactory.CreateRectEntity(...)` | Same as `AddRect` | The new entity |

Each function accepts `ColorRGB` or `ColorRGBA` as the color argument.

**Example**

```lua
local e = EntityFactory.CreateCircleEntity("coin", world_id,
        vec2f(10, 10), 0.5, ColorRGBA(255, 215, 0, 255), false, true)
```

**WARNING**
The return type of `CreateCircleEntity` and `CreateRectEntity` comes from C++. If the type is `std::unique_ptr<Entity>`, sol2 cannot copy it. A Lua variable then gets a unique ownership. Do not use the returned value in two places.

---

## 8. Complete Example

This script makes the ground and two walls. It needs the globals `game_window` and `world_id`.

```lua
local WALL_THICKNESS = 32.0
local STATIC_BODY = true
local USE_METERS = false
local WHITE = ColorRGB(255, 255, 255)

local res_x = game_window.resolution.x
local res_y = game_window.resolution.y

local ground_size = vec2f(res_x, 32.0)
local ground_pos  = vec2f(ground_size.x * 0.5, res_y - ground_size.y * 0.5)
Entman.AddRect("Ground", world_id, ground_pos, ground_size,
               WHITE, STATIC_BODY, USE_METERS)

local wall_size = vec2f(WALL_THICKNESS, res_y)
Entman.AddRect("LeftWall", world_id,
               vec2f(WALL_THICKNESS * 0.5, res_y * 0.5), wall_size,
               WHITE, STATIC_BODY, USE_METERS)
Entman.AddRect("RightWall", world_id,
               vec2f(res_x - WALL_THICKNESS * 0.5, res_y * 0.5), wall_size,
               WHITE, STATIC_BODY, USE_METERS)
```

---

## 9. Known Limits

- `Entman` has no Lua instance. Add one in C++ to use `MakeEntity` and `RemoveEntity`.
- `b2WorldId` must be registered in a different place, and `world_id` must be set in C++.
- `vec2`, `uvec2`, and `ColorRGB` have no operators. Only `vec2f` has operators.
- `ColorRGB` and `ColorRGBA` have no `tostring` function.
- `uvec2` values are unsigned. A negative value gives a wrong large number.
- The comment "static: use a dot, not a colon" in `RegisterVec2f` is not correct. `pixelsToMeters` and `metersToPixels` take the vector as the first argument. Call them with a colon. A dot call also works if you pass the vector: `vec2f.pixelsToMeters(v)`.