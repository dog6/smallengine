-- used as sanity check
-- "when all else fails, this will put a plane on the screen"

print("--- basic_demo.lua started ---")

function CreateBox(name, pos, size) 
    local box = entman:MakeEntity(name)
    box:AddComponent("DrawableComponent")
    box:AddComponent("PhysicsComponent")







-- -- local scene = game:GetActiveScene() -- get active scene

-- -- Helper function
-- -- function CreateEntity() ?

-- -- function CreateMeshObject(name, modelPath, diffusePath)
-- --     local go = scene:CreateGameObject(name)
-- --     go:AddComponent("MeshRenderer")
-- --     local renderer = go:GetComponent("MeshRenderer")
-- --     renderer:LoadMesh(modelPath)

-- --     -- Load the diffuse texture only if a path is given
-- --     if diffusePath ~= nil then
-- --         renderer:SetTexture("diffuse", diffusePath)
-- --     end

-- --     return go
-- -- end

-- -- Create gameObject 'plane_lower'
-- local plGO = CreateRectEntity()

-- local plTF = plGO:GetComponent("Transform") -- reference Transform component
-- plTF:SetPosition(0, -1, 0) -- set transform position

-- -- Unused, just here
-- function OnUpdate(deltaTime)
-- end