-- lua file for a simple block
-- could change this to a render only .obj file

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

s0 = gr.mesh('sphere','s0')
s0:scale(0.15,0.15,0.15)
-- s0:translate(-0.5,-0.0,-0.5)
rootnode:add_child(s0)
s0:set_material(red)

return rootnode
