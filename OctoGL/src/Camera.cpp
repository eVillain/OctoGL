#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>

inline static float toRadians( const float &theAngleInDegrees ) {
    return theAngleInDegrees * M_PI / 180.0f;
}

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
    return std::max(lower, std::min(n, upper));
}

Camera::Camera()
{
    fieldOfView = 90.0f;
    nearDepth = 0.01f;
    farDepth = 1000.0f;
    
    shakeVect = glm::vec3();
    shakeAmount = 0.0f;
    shakeDecay = 0.85f;
    
    _position = glm::vec3(0.0f,0.0f,0.0f);
    _rotation = glm::vec3(0.0f,0.0f,0.0f);
    rotationSensitivity = glm::vec2(20.0f,20.0f);
    speed = glm::vec3(0.0f, 0.0f, 0.0f);
    movement = glm::vec3(0.0f,0.0f,0.0f);
    movementSpeedFactor = 1.0f;
    
    targetPosition = glm::vec3(0.0f,0.0f,0.0f);
    targetRotation = glm::vec3(0.0f,0.0f,0.0f);
    elasticMovement = false;
    autoRotate = false;
    thirdPerson = false;
    elasticity = 20.0f;
    distance = 6.0f;
    maxDistance = 20.0f;
    height = 2.0f;
}

Camera::~Camera()
{ }

void Camera::Update( double delta )
{
    if ( delta <= 0.0 ) return;
    if ( delta >= 1.0 ) return;

    if ( shakeVect.x != 0.0f || shakeVect.y != 0.0f ) {
        _position -= shakeVect;
    }
    if ( elasticMovement ) {
        if ( targetRotation.y - _rotation.y < -180.0f ) targetRotation.y += 360.0f;
        else if ( targetRotation.y - _rotation.y > 180.0f ) targetRotation.y -= 360.0f;

        // Dampen rotation
        float new_ratio = elasticity*0.02f * delta;
        float old_ratio = 1.0 - new_ratio;
        glm::vec3 new_dir = (_rotation * old_ratio) + (targetRotation * new_ratio);
        _rotation = new_dir;
        
        if ( glm::length(movement) > 0.0f ) {
            // Apply direct camera movement
            CalculateCameraMovement( movement );
            targetPosition += speed*float(delta);
        }
        if ( thirdPerson ) {
            // Find coordinate for camera
            glm::vec3 zVect = glm::vec3(0.0f,0.0f,1.0f);
            glm::vec3 zPos = zVect*distance;
            zPos = glm::rotateX(zPos, _rotation.x);
            zPos = glm::rotateY(zPos, _rotation.y);
            zPos = glm::rotateZ(zPos, -_rotation.z);
            glm::vec3 behindTarget = targetPosition + zPos;
            behindTarget.y += height;
            glm::vec3 move = behindTarget - _position;
            float dist = glm::length(move);
            if ( dist > 20.0f ) {
                move = glm::normalize(move)*20.0f;
            }
            speed = move*(elasticity*0.1f)*float(delta);
            
            _position = _position+speed;
        } else {
            glm::vec3 move = targetPosition - _position;
            speed = move*elasticity*0.1f*float(delta);
            _position = _position+speed;
        }
    } else {
        CalculateCameraMovement( movement );
        _rotation = targetRotation;
        _position = _position+speed*float(delta);
    }
    // Shake disabled for now, this requires a random number generator :)
//    if ( shakeAmount != 0.0f ) {
//        Random::RandomSeed(int(Timer::Microseconds()));
//        int rX = Random::RandomInt(-1, 1);
//        int rY = Random::RandomInt(-1, 1);
//        shakeVect = glm::vec3(rX, rY, 0.0f)*shakeAmount;
//        _position += shakeVect;
//        shakeAmount *= shakeDecay;
//        if ( fabsf(shakeAmount) < 0.01f ) { shakeAmount = 0.0f; shakeVect = glm::vec3(); }
//    }
    UpdateMatrices();
//    printf("Camera: %f,%f,%f - %f,%f,%f\n", _position.x, _position.y, _position.z, rotation.x, rotation.y, rotation.z);
}

void Camera::CameraRotate( const float rotX, const float rotY )
{

    _rotation.x += rotY;
    _rotation.y += rotX;
    
    // Control looking up and down with the mouse forward/back movement
    _rotation.x = clamp<float>(_rotation.x, -M_PI_2, M_PI_2);
    
    // Looking left and right. Keep the angles in the range -180.0f (anticlockwise turn looking behind) to 180.0f (clockwise turn looking behind)
    if (_rotation.y < -M_PI) {
        _rotation.y += 2*M_PI;
    }
    if (_rotation.y > M_PI) {
        _rotation.y -= 2*M_PI;
    }
    targetRotation = _rotation;
}

// Function to calculate which direction we need to move the camera and by what amount
void Camera::CalculateCameraMovement(const glm::vec3& direction) {
    // Break up our movement into components along the X, Y and Z axis
    float camMovementXComponent = 0.0f;
    float camMovementYComponent = 0.0f;
    float camMovementZComponent = 0.0f;
    // Forward/backward movement
    if (direction.z != 0.0f) {
        // Control X-Axis movement
        float pitchFactor = cos(_rotation.x);
        camMovementXComponent += (movementSpeedFactor * float(sin((_rotation.y))) * direction.z) * pitchFactor;
        // Control Y-Axis movement
        camMovementYComponent += movementSpeedFactor * float(sin((_rotation.x)) * -direction.z);
        // Control Z-Axis movement
        float yawFactor = (cos((_rotation.x)));
        camMovementZComponent += (movementSpeedFactor * float(cos((_rotation.y))) * direction.z) * yawFactor;
    }
    // Strafing
    if ( direction.x !=  0.0f ) {
        // Calculate our Y-Axis _rotation in radians once here because we use it twice
        float yRotRad = (_rotation.y);
        camMovementXComponent +=  movementSpeedFactor * float(cos(yRotRad)) * direction.x;
        camMovementZComponent +=  movementSpeedFactor * float(sin(yRotRad)) * -direction.x;
    }
    // Vertical movement
    if ( direction.y !=  0.0f ) {
        float xRotRad = toRadians(_rotation.x);
        float yRotRad = toRadians(_rotation.y);
        camMovementYComponent +=  movementSpeedFactor * float(cos(xRotRad)) * -direction.y;
        camMovementXComponent +=  movementSpeedFactor * float(sin(yRotRad)) * direction.y;
        camMovementZComponent +=  movementSpeedFactor * float(cos(yRotRad)) * direction.y;
    }
    // After combining our movements for any & all keys pressed, assign them to our camera speed along the given axis
    speed.x = camMovementXComponent;
    speed.y = camMovementYComponent;
    speed.z = camMovementZComponent;
    
    // Cap the speeds to our movementSpeedFactor (otherwise moving diagonally will be faster than along an axis!)
    speed.x = clamp(speed.x, -movementSpeedFactor, movementSpeedFactor);
    speed.y = clamp(speed.y, -movementSpeedFactor, movementSpeedFactor);
    speed.z = clamp(speed.z, -movementSpeedFactor, movementSpeedFactor);
}

// Function to move the camera the amount we've calculated in the calculateCameraMovement function
void Camera::MoveCamera( double dt ) {
    _position += speed*(float)dt;
}

void Camera::UpdateMatrices()
{
    float aspectRatio = (_viewSizePixels.x > _viewSizePixels.y) ?
        (_viewSizePixels.x / _viewSizePixels.y) :
        (_viewSizePixels.y / _viewSizePixels.x);

    _model = glm::mat4();
    _model = glm::rotate(_model, -_rotation.x, glm::vec3(1.0, 0.0, 0.0));
    _model = glm::rotate(_model, -_rotation.y, glm::vec3(0.0, 1.0, 0.0));
    _model = glm::rotate(_model, -_rotation.z, glm::vec3(0.0, 0.0, 1.0));
    _model = glm::translate(_model, glm::vec3(-_position.x, -_position.y, -_position.z));
    
    _projection = glm::mat4();
    _projection = glm::perspective(fieldOfView, aspectRatio, nearDepth, farDepth);
    _viewport = glm::vec4(0, 0, _viewSizePixels.x, _viewSizePixels.y);
    
    _mvp = _projection*_model;
}
