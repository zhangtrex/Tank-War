
m1 = gr.material({0.06, 0.31, 0.5}, {0.06, 0.31, 0.5}, 10.0)

rootNode = gr.node('root')
rootNode:scale(0.2,0.2,0.2)

bodyJoint = gr.joint('bodyJoint', {0,0,0}, {0,0,0})
bodyJoint:translate(0.4,0,0)
rootNode:add_child(bodyJoint)

bodyMesh = gr.mesh('cube', 'bodyMesh')
bodyMesh:scale(4.2,0.65,2.5)
bodyMesh:translate(-0.5, 0, 0)
bodyMesh:set_material(m1)
bodyJoint:add_child(bodyMesh)



wheel1Joint = gr.joint('wheel1Joint', {0,0,0}, {0,0,0})
wheel1Joint:translate(0,0,0.8)
bodyJoint:add_child(wheel1Joint)

wheel1Mesh1 = gr.mesh('cube', 'wheel1Mesh1')
wheel1Mesh1:scale(2,0.6, 0.5)
wheel1Mesh1:translate(0,-0.6,0)
wheel1Mesh1:set_material(m1)
wheel1Joint:add_child(wheel1Mesh1)

wheel1Joint1 = gr.joint('wheel1Joint1', {0,0,0},{0,0,0})
wheel1Joint1:translate(1.13,-0.375,0)
wheel1Joint:add_child(wheel1Joint1)

wheel1Mesh2 = gr.mesh('cube', 'wheel1Mesh2')
wheel1Mesh2:scale(1,0.4,0.5)
wheel1Mesh2:rotate('z',55)
wheel1Mesh2:set_material(m1)
wheel1Joint1:add_child(wheel1Mesh2)

wheel1Joint2 = gr.joint('wheel1Joint2', {0,0,0}, {0,0,0})
wheel1Joint2:translate(-1.65,-0.29,0)
wheel1Joint:add_child(wheel1Joint2)

wheel1Mesh3 = gr.mesh('cube', 'wheel1Mesh3')
wheel1Mesh3:scale(1.7,0.55,0.5)
wheel1Mesh3:rotate('z', -25)
wheel1Mesh3:set_material(m1)
wheel1Joint2:add_child(wheel1Mesh3)



wheel2Joint = gr.joint('wheel2Joint', {0,0,0}, {0,0,0})
wheel2Joint:translate(0,0,-0.8)
bodyJoint:add_child(wheel2Joint)

wheel2Mesh1 = gr.mesh('cube', 'wheel2Mesh1')
wheel2Mesh1:scale(2,0.6, 0.5)
wheel2Mesh1:translate(0,-0.6,0)
wheel2Mesh1:set_material(m1)
wheel2Joint:add_child(wheel2Mesh1)

wheel2Joint1 = gr.joint('wheel2Joint1', {0,0,0},{0,0,0})
wheel2Joint1:translate(1.13,-0.375,0)
wheel2Joint:add_child(wheel2Joint1)

wheel2Mesh2 = gr.mesh('cube', 'wheel2Mesh2')
wheel2Mesh2:scale(1,0.4,0.5)
wheel2Mesh2:rotate('z',55)
wheel2Mesh2:set_material(m1)
wheel2Joint1:add_child(wheel2Mesh2)

wheel2Joint2 = gr.joint('wheel2Joint2', {0,0,0}, {0,0,0})
wheel2Joint2:translate(-1.65,-0.29,0)
wheel2Joint:add_child(wheel2Joint2)

wheel2Mesh3 = gr.mesh('cube', 'wheel2Mesh3')
wheel2Mesh3:scale(1.7,0.55,0.5)
wheel2Mesh3:rotate('z', -25)
wheel2Mesh3:set_material(m1)
wheel2Joint2:add_child(wheel2Mesh3)

bodyMesh1 = gr.mesh('sphere', 'bodyMesh1')
bodyMesh1:scale(1,0.75,0.75)
bodyMesh1:translate(0.3,0.4,0)
bodyMesh1:set_material(m1)
bodyJoint:add_child(bodyMesh1)

gunJoint = gr.joint('gunJoint', {0,0,0}, {0,0,0})
gunJoint:rotate('z', -10)
gunJoint:translate(0.3, 0.4, 0)
bodyJoint:add_child(gunJoint)

gunMesh = gr.mesh('cube', 'gunMesh')
gunMesh:scale(3,0.3,0.3)
gunMesh:translate(-1.5,0,0)
gunMesh:set_material(m1)
gunJoint:add_child(gunMesh)

return rootNode
