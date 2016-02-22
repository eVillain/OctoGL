#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    virtual ~Camera();
    
    void Update( double delta );
    
    /// Deals with mouse position changes, called whenever the mouse cursor moves
    void CameraRotate(const float rotX,
                      const float rotY);
    
    /// Calculates which direction we need to move the camera and by what amount
    void CalculateCameraMovement(const glm::vec3& movement);
    
    /// Moves the camera by the amount calculated in calculateCameraMovement
    void MoveCamera(double delta);
    
    void setPosition(const glm::vec3 position) { _position = position; };
    void setRotation(const glm::vec3 rotation) { _rotation = rotation; targetRotation = rotation; };
    void setViewSize(const glm::vec2 viewSize) { _viewSizePixels = viewSize; };

    const glm::vec3 getPosition() const { return _position; }
    const glm::vec3 getRotation() const { return _rotation; }
    const glm::vec2 getViewSize() const { return _viewSizePixels; }

    const glm::mat4& getModel() const { return _model; };
    const glm::vec4& getViewport() const { return _viewport; };
    const glm::mat4& getProjection() const { return _projection; };
    const glm::mat4& getMVP() const { return _mvp; };
  
private:
    void UpdateMatrices();
    
    glm::mat4 _model;
    glm::vec4 _viewport;
    glm::mat4 _projection;
    glm::mat4 _mvp;
    
    glm::ivec2 _viewSizePixels;
    
    // Direct manipulation:
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec2 rotationSensitivity;
    glm::vec3 movement;
    glm::vec3 speed;
    
    glm::vec3 shakeVect;
    float shakeDecay;
    float shakeAmount;
    
    // Elastic motion
    glm::vec3 targetPosition;
    glm::vec3 targetRotation;
    bool elasticMovement;
    bool autoRotate;
    float elasticity;
    
    bool thirdPerson;
    float distance;
    float maxDistance;
    float height;
    
    float movementSpeedFactor;    // How fast we move (higher values mean we move and strafe faster)
    float fieldOfView;            // Define our field of view (i.e. how quickly foreshortening occurs)
    float nearDepth;              // The near (Z Axis) point of our viewing frustrum (default 0.01f)
    float farDepth;               // The far  (Z Axis) point of our viewing frustrum (default 1000.0f)
};

#endif
