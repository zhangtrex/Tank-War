-- lua file for a simple block
-- could change this to a render only .obj file

rootnode = gr.node('root')

m1 = gr.material({0.2, 0.3, 0.2}, {0.1, 0.1, 0.1}, 10)


s0 = gr.mesh('cube','s0')
rootnode:add_child(s0)
s0:set_material(m1)

return rootnode
