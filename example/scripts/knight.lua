local speed = 1.5
local direction = 0

function start()
end

function update(delta)
    transform = entity.get_component("transform")
    animation = entity.get_component("animator")

    if window.get_key_down("W") then
        direction = 2
        transform.translate(0, 0, - speed * delta)
        transform.set_rotation(0, 180, 0)
        animation.set_animation("knight_walk");
    elseif window.get_key_down("S") then
        direction = 0
        transform.translate(0, 0, speed * delta)
        transform.set_rotation(0, 0, 0)
        animation.set_animation("knight_walk");
    elseif window.get_key_down("A") then
        direction = 1
        transform.translate(- speed * delta, 0, 0)
        transform.set_rotation(0, -90, 0)
        animation.set_animation("knight_walk");
    elseif window.get_key_down("D") then
        direction = 3
        transform.translate(speed * delta, 0, 0)
        transform.set_rotation(0, 90, 0)
        animation.set_animation("knight_walk");
    else
        animation.set_animation("knight_idle");
    end
end

function on_collision(distance)
    transform = entity.get_component("transform")
    animation = entity.get_component("animator")
    
    if direction == 0 then
        transform.translate(0, 0, distance)
    elseif direction == 1 then
        transform.translate(-distance, 0, 0)
    elseif direction == 2 then
        transform.translate(0, 0, -distance)
    elseif direction == 3 then
        transform.translate(distance, 0, 0)
    end
    animation.set_animation("knight_idle");
end