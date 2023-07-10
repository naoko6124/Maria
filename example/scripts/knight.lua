local speed = 1.5
local transform
local animation

function start()
    transform = entity.get_component("transform")
    animation = entity.get_component("animator")
end

function update(delta)
    if window.get_key_down("W") then
        transform.translate(0, 0, - speed * delta)
        transform.set_rotation(0, 180, 0)
        animation.set_animation("knight_walk");
    elseif window.get_key_down("S") then
        transform.translate(0, 0, speed * delta)
        transform.set_rotation(0, 0, 0)
        animation.set_animation("knight_walk");
    elseif window.get_key_down("A") then
        transform.translate(- speed * delta, 0, 0)
        transform.set_rotation(0, -90, 0)
        animation.set_animation("knight_walk");
    elseif window.get_key_down("D") then
        transform.translate(speed * delta, 0, 0)
        transform.set_rotation(0, 90, 0)
        animation.set_animation("knight_walk");
    else
        animation.set_animation("knight_idle");
    end
end

function on_collision(distance)
    print("collision: "..distance)
    transform.translate(0, 0, -distance)
end