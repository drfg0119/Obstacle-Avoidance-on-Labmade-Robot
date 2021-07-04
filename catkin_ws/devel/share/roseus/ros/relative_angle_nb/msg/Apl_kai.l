;; Auto-generated. Do not edit!


(when (boundp 'relative_angle_nb::Apl_kai)
  (if (not (find-package "RELATIVE_ANGLE_NB"))
    (make-package "RELATIVE_ANGLE_NB"))
  (shadow 'Apl_kai (find-package "RELATIVE_ANGLE_NB")))
(unless (find-package "RELATIVE_ANGLE_NB::APL_KAI")
  (make-package "RELATIVE_ANGLE_NB::APL_KAI"))

(in-package "ROS")
;;//! \htmlinclude Apl_kai.msg.html


(defclass relative_angle_nb::Apl_kai
  :super ros::object
  :slots (_obs_dist _obs_direct _goal_x _goal_y _obs_latelitude ))

(defmethod relative_angle_nb::Apl_kai
  (:init
   (&key
    ((:obs_dist __obs_dist) 0)
    ((:obs_direct __obs_direct) 0.0)
    ((:goal_x __goal_x) 0.0)
    ((:goal_y __goal_y) 0.0)
    ((:obs_latelitude __obs_latelitude) 0)
    )
   (send-super :init)
   (setq _obs_dist (round __obs_dist))
   (setq _obs_direct (float __obs_direct))
   (setq _goal_x (float __goal_x))
   (setq _goal_y (float __goal_y))
   (setq _obs_latelitude (round __obs_latelitude))
   self)
  (:obs_dist
   (&optional __obs_dist)
   (if __obs_dist (setq _obs_dist __obs_dist)) _obs_dist)
  (:obs_direct
   (&optional __obs_direct)
   (if __obs_direct (setq _obs_direct __obs_direct)) _obs_direct)
  (:goal_x
   (&optional __goal_x)
   (if __goal_x (setq _goal_x __goal_x)) _goal_x)
  (:goal_y
   (&optional __goal_y)
   (if __goal_y (setq _goal_y __goal_y)) _goal_y)
  (:obs_latelitude
   (&optional __obs_latelitude)
   (if __obs_latelitude (setq _obs_latelitude __obs_latelitude)) _obs_latelitude)
  (:serialization-length
   ()
   (+
    ;; uint16 _obs_dist
    2
    ;; float32 _obs_direct
    4
    ;; float32 _goal_x
    4
    ;; float32 _goal_y
    4
    ;; int32 _obs_latelitude
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint16 _obs_dist
       (write-word _obs_dist s)
     ;; float32 _obs_direct
       (sys::poke _obs_direct (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _goal_x
       (sys::poke _goal_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _goal_y
       (sys::poke _goal_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; int32 _obs_latelitude
       (write-long _obs_latelitude s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint16 _obs_dist
     (setq _obs_dist (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; float32 _obs_direct
     (setq _obs_direct (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _goal_x
     (setq _goal_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _goal_y
     (setq _goal_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; int32 _obs_latelitude
     (setq _obs_latelitude (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get relative_angle_nb::Apl_kai :md5sum-) "9809d650616fd3c284b0b4b27cd2e9fe")
(setf (get relative_angle_nb::Apl_kai :datatype-) "relative_angle_nb/Apl_kai")
(setf (get relative_angle_nb::Apl_kai :definition-)
      "uint16 obs_dist
float32 obs_direct
float32 goal_x
float32 goal_y
int32 obs_latelitude
")



(provide :relative_angle_nb/Apl_kai "9809d650616fd3c284b0b4b27cd2e9fe")


