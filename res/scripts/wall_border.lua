-- Makes the ground, the left wall, and the right wall.
-- All three bodies are static. All three use pixel units.
 
local WALL_THICKNESS = 32.0
local GROUND_THICKNESS = 32.0
local STATIC_BODY = true
local USE_METERS = false   -- false = pixel scale (USE_PIXEL_SCALE in C++)
 
local WHITE = ColorRGB(255, 255, 255)
 
-- window:   the GameWindow object
-- world_id: the b2WorldId of the physics world
function create_boundaries(window, world_id)
    local res_x = window.resolution.x
    local res_y = window.resolution.y
 
    -- Ground: as wide as the window, at the bottom edge
    local ground_size = vec2f(res_x, GROUND_THICKNESS)
    local ground_pos  = vec2f(ground_size.x * 0.5,
                              res_y - ground_size.y * 0.5)
 
    Entman.AddRect("Ground", world_id, ground_pos, ground_size,
                   WHITE, STATIC_BODY, USE_METERS)
 
    -- Walls: as tall as the window
    local wall_size = vec2f(WALL_THICKNESS, res_y)
    local left_pos  = vec2f(WALL_THICKNESS * 0.5, res_y * 0.5)
    local right_pos = vec2f(res_x - WALL_THICKNESS * 0.5, res_y * 0.5)
 
    Entman.AddRect("LeftWall", world_id, left_pos, wall_size,
                   WHITE, STATIC_BODY, USE_METERS)
 
    Entman.AddRect("RightWall", world_id, right_pos, wall_size,
                   WHITE, STATIC_BODY, USE_METERS)
end
 
-- Run it. These two globals must come from C++:
--   game_window : GameWindow
--   world_id    : b2WorldId
create_boundaries(game_window, world_id)