
rootNode = gr.node('root')
rootNode:scale(0.35,0.35,0.35)
rootNode:rotate('y',30)

shoulderJoint = gr.joint('shoulderJoint', {-30, 0, 30}, {0, 0, 0})
shoulderJoint:translate(0, 0.9, 0)
shoulderJoint:rotate('z', 0)
rootNode:add_child(shoulderJoint)

hipJoint = gr.joint('hipJoint', {-30, 0, 30}, {0, 0, 0})
hipJoint:translate(0, -0.9, 0)
hipJoint:rotate('z', 0)
rootNode:add_child(hipJoint)


-- legs

leftLegHipJoint = gr.joint('leftLegHipJoint', {-60, 0, 45}, {0, 0, 0})
leftLegHipJoint:translate(0.25, 0, 0)
hipJoint:add_child(leftLegHipJoint)

leftLegLowerJoint = gr.joint('leftLegLowerJoint', {0, 0, 60}, {0, 0, 0})
leftLegLowerJoint:translate(0, -0.9, 0)
leftLegHipJoint:add_child(leftLegLowerJoint)

leftFootJoint = gr.joint('leftFootJoint', {0, 0, 0}, {-30, 0, 30})
leftFootJoint:translate(0, -0.9, 0)
leftLegLowerJoint:add_child(leftFootJoint)

leftLegMesh = gr.mesh('cube', 'leftLegMesh')
leftLegMesh:scale(0.4, 1, 0.4)
leftLegMesh:translate(0, -0.5, 0)
leftLegMesh:set_material(gr.material({0.2, 0.8, 0.2}, {0.8, 0.8, 0.8}, 10.0))
leftLegHipJoint:add_child(leftLegMesh)

leftLowerLegMesh = gr.mesh('cube', 'leftLowerLegMesh')
leftLowerLegMesh:scale(0.35, 1, 0.35)
leftLowerLegMesh:translate(0, -0.5, 0)
leftLowerLegMesh:set_material(gr.material({0.2, 0.8, 0.2}, {0.8, 0.8, 0.8}, 10.0))
leftLegLowerJoint:add_child(leftLowerLegMesh)

leftFootMesh = gr.mesh('cube', 'leftFootMesh')
leftFootMesh:scale(0.4, 0.2, 0.6)
leftFootMesh:translate(0, -0.1, 0.1)
leftFootMesh:set_material(gr.material({0.2, 0.2, 0.8}, {0.8, 0.8, 0.8}, 10.0))
leftFootJoint:add_child(leftFootMesh)


rightLegHipJoint = gr.joint('rightLegHipJoint', {-60, 0, 45}, {0, 0, 0})
rightLegHipJoint:translate(-0.25, 0, 0)
hipJoint:add_child(rightLegHipJoint)

rightLegLowerJoint = gr.joint('rightLegLowerJoint', {0, 0, 60}, {0, 0, 0})
rightLegLowerJoint:translate(0, -0.9, 0)
rightLegHipJoint:add_child(rightLegLowerJoint)

rightFootJoint = gr.joint('rightFootJoint', {0, 0, 0}, {-30, 0, 30})
rightFootJoint:translate(0, -0.9, 0)
rightLegLowerJoint:add_child(rightFootJoint)

rightLegMesh = gr.mesh('cube', 'rightLegMesh')
rightLegMesh:scale(0.4, 1, 0.4)
rightLegMesh:translate(0, -0.5, 0)
rightLegMesh:set_material(gr.material({0.2, 0.8, 0.2}, {0.8, 0.8, 0.8}, 10.0))
rightLegHipJoint:add_child(rightLegMesh)

rightLowerLegMesh = gr.mesh('cube', 'rightLowerLegMesh')
rightLowerLegMesh:scale(0.35, 1, 0.35)
rightLowerLegMesh:translate(0, -0.5, 0)
rightLowerLegMesh:set_material(gr.material({0.2, 0.8, 0.2}, {0.8, 0.8, 0.8}, 10.0))
rightLegLowerJoint:add_child(rightLowerLegMesh)

rightFootMesh = gr.mesh('cube', 'rightFootMesh')
rightFootMesh:scale(0.4, 0.2, 0.6)
rightFootMesh:translate(0, -0.1, 0.1)
rightFootMesh:set_material(gr.material({0.2, 0.2, 0.8}, {0.8, 0.8, 0.8}, 10.0))
rightFootJoint:add_child(rightFootMesh)



-- arm

leftArmShoulderJoint = gr.joint('leftArmShoulderJoint', {0, 0, 0}, {-45, 0, 180})
leftArmShoulderJoint:translate(0, -0.7, 0)
leftArmShoulderJoint:rotate('z', 90)
shoulderJoint:add_child(leftArmShoulderJoint)

leftArmLowerJoint = gr.joint('leftArmLowerJoint', {0, 0, 0}, {0, 0, 60})
leftArmLowerJoint:translate(-1, 0, 0)
leftArmShoulderJoint:add_child(leftArmLowerJoint)

leftHandJoint = gr.joint('leftHandJoint', {-90, 0, 90}, {0, 0, 0})
leftHandJoint:translate(-1.1, 0, 0)
leftArmLowerJoint:add_child(leftHandJoint)

leftArmMesh = gr.mesh('cube', 'leftArmMesh')
leftArmMesh:scale(1, 0.4, 0.4)
leftArmMesh:translate(-0.5,0,0)
leftArmMesh:set_material(gr.material({0.2, 0.8, 0.2}, {0.8, 0.8, 0.8}, 10.0))
leftArmShoulderJoint:add_child(leftArmMesh)

leftLowerArmMesh = gr.mesh('cube', 'leftLowerArmMesh')
leftLowerArmMesh:scale(1, 0.3, 0.3)
leftLowerArmMesh:translate(-0.5,0,0)
leftLowerArmMesh:set_material(gr.material({0.2, 0.8, 0.2}, {0.8, 0.8, 0.8}, 10.0))
leftArmLowerJoint:add_child(leftLowerArmMesh)

leftHandMesh = gr.mesh('sphere', 'leftHandMesh')
leftHandMesh:scale(0.3, 0.1, 0.3)
leftHandMesh:translate(0,0,0)
leftHandMesh:set_material(gr.material({0.2, 0.2, 0.8}, {0.8, 0.8, 0.8}, 10.0))
leftHandJoint:add_child(leftHandMesh)


rightArmShoulderJoint = gr.joint('rightArmShoulderJoint', {0, 0, 0}, {-45, 0,180})
rightArmShoulderJoint:translate(0, 0.7, 0)
rightArmShoulderJoint:rotate('z', 90)
shoulderJoint:add_child(rightArmShoulderJoint)

rightArmLowerJoint = gr.joint('rightArmLowerJoint', {0, 0, 0}, {0, 0, 60})
rightArmLowerJoint:translate(-1, 0, 0)
rightArmShoulderJoint:add_child(rightArmLowerJoint)

rightHandJoint = gr.joint('rightHandJoint', {-90, 0, 90}, {0, 0, 0})
rightHandJoint:translate(-1.1, 0, 0)
rightArmLowerJoint:add_child(rightHandJoint)

rightArmMesh = gr.mesh('cube', 'rightArmMesh')
rightArmMesh:scale(1, 0.4, 0.4)
rightArmMesh:translate(-0.5,0,0)
rightArmMesh:set_material(gr.material({0.2, 0.8, 0.2}, {0.8, 0.8, 0.8}, 10.0))
rightArmShoulderJoint:add_child(rightArmMesh)

rightLowerArmMesh = gr.mesh('cube', 'rightLowerArmMesh')
rightLowerArmMesh:scale(1, 0.3, 0.3)
rightLowerArmMesh:translate(-0.5,0,0)
rightLowerArmMesh:set_material(gr.material({0.2, 0.8, 0.2}, {0.8, 0.8, 0.8}, 10.0))
rightArmLowerJoint:add_child(rightLowerArmMesh)

rightHandMesh = gr.mesh('sphere', 'rightHandMesh')
rightHandMesh:scale(0.3, 0.1, 0.3)
rightHandMesh:translate(0,0,0)
rightHandMesh:set_material(gr.material({0.2, 0.2, 0.8}, {0.8, 0.8, 0.8}, 10.0))
rightHandJoint:add_child(rightHandMesh)



-- head & body & eyes & headphone

neckJoint = gr.joint('neckJoint', {-15, 0, 30}, {0, 0, 0})
neckJoint:translate(0, 0.1, 0)
neckJoint:rotate('x', -30)
shoulderJoint:add_child(neckJoint)

headJoint = gr.joint('headJoint', {-18, 0, 18}, {-45, 0, 45})
headJoint:translate(0, 0.2, 0)
headJoint:rotate('x', 0)
neckJoint:add_child(headJoint)

cubeMesh = gr.mesh('cube', '-body')
cubeMesh:scale(1.0, 2.0, 0.5)
cubeMesh:rotate('y', 0)
cubeMesh:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
rootNode:add_child(cubeMesh)

neckMesh = gr.mesh('sphere', 'neck')
neckMesh:scale(0.2, 0.2, 0.2)
neckMesh:translate(0, 0.1, 0)
neckMesh:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
neckJoint:add_child(neckMesh)

headMesh = gr.mesh('cube', 'head')
headMesh:scale(0.6, 0.6, 0.6)
headMesh:translate(0, 0.3, 0)
headMesh:set_material(gr.material({0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0))
headJoint:add_child(headMesh)

leftEyeMesh = gr.mesh('cube', '-leftEyeMesh')
leftEyeMesh:scale(0.2,0.1,0.1)
leftEyeMesh:translate(0.15, 0.4, 0.26)
leftEyeMesh:set_material(gr.material({0.95, 0.95, 0.95}, {0.8, 0.8, 0.8}, 10.0))
headJoint:add_child(leftEyeMesh)

rightEyeMesh = gr.mesh('cube', '-rightEyeMesh')
rightEyeMesh:scale(0.2,0.1,0.1)
rightEyeMesh:translate(-0.15, 0.4, 0.26)
rightEyeMesh:set_material(gr.material({0.95, 0.95, 0.95}, {0.8, 0.8, 0.8}, 10.0))
headJoint:add_child(rightEyeMesh)

leftEye1Mesh = gr.mesh('cube', '-leftEye1Mesh')
leftEye1Mesh:scale(0.1, 0.08, 0.1)
leftEye1Mesh:translate(0.15, 0.4, 0.27)
leftEye1Mesh:set_material(gr.material({0.2, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
headJoint:add_child(leftEye1Mesh)

rightEye1Mesh = gr.mesh('cube', '-rightEye1Mesh')
rightEye1Mesh:scale(0.1, 0.08, 0.1)
rightEye1Mesh:translate(-0.15, 0.4, 0.27)
rightEye1Mesh:set_material(gr.material({0.2, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
headJoint:add_child(rightEye1Mesh)

earphoneMesh = gr.mesh('cube', '-earphoneMesh')
earphoneMesh:scale(0.8, 0.2, 0.2)
earphoneMesh:translate(0, 0.3, 0)
earphoneMesh:set_material(gr.material({0.2, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
headJoint:add_child(earphoneMesh)

earphone1Mesh = gr.mesh('cube', '-earphone1Mesh')
earphone1Mesh:scale(0.7, 0.4, 0.15)
earphone1Mesh:translate(0, 0.5, 0)
earphone1Mesh:set_material(gr.material({0.2, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
headJoint:add_child(earphone1Mesh)

return rootNode
