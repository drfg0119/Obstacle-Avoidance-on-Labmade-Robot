; Auto-generated. Do not edit!


(cl:in-package relative_angle_nb-msg)


;//! \htmlinclude Apl.msg.html

(cl:defclass <Apl> (roslisp-msg-protocol:ros-message)
  ((obs_dist
    :reader obs_dist
    :initarg :obs_dist
    :type cl:fixnum
    :initform 0)
   (obs_direct
    :reader obs_direct
    :initarg :obs_direct
    :type cl:float
    :initform 0.0)
   (goal_x
    :reader goal_x
    :initarg :goal_x
    :type cl:float
    :initform 0.0)
   (goal_y
    :reader goal_y
    :initarg :goal_y
    :type cl:float
    :initform 0.0)
   (obs_latelitude
    :reader obs_latelitude
    :initarg :obs_latelitude
    :type cl:integer
    :initform 0))
)

(cl:defclass Apl (<Apl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Apl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Apl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name relative_angle_nb-msg:<Apl> is deprecated: use relative_angle_nb-msg:Apl instead.")))

(cl:ensure-generic-function 'obs_dist-val :lambda-list '(m))
(cl:defmethod obs_dist-val ((m <Apl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader relative_angle_nb-msg:obs_dist-val is deprecated.  Use relative_angle_nb-msg:obs_dist instead.")
  (obs_dist m))

(cl:ensure-generic-function 'obs_direct-val :lambda-list '(m))
(cl:defmethod obs_direct-val ((m <Apl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader relative_angle_nb-msg:obs_direct-val is deprecated.  Use relative_angle_nb-msg:obs_direct instead.")
  (obs_direct m))

(cl:ensure-generic-function 'goal_x-val :lambda-list '(m))
(cl:defmethod goal_x-val ((m <Apl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader relative_angle_nb-msg:goal_x-val is deprecated.  Use relative_angle_nb-msg:goal_x instead.")
  (goal_x m))

(cl:ensure-generic-function 'goal_y-val :lambda-list '(m))
(cl:defmethod goal_y-val ((m <Apl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader relative_angle_nb-msg:goal_y-val is deprecated.  Use relative_angle_nb-msg:goal_y instead.")
  (goal_y m))

(cl:ensure-generic-function 'obs_latelitude-val :lambda-list '(m))
(cl:defmethod obs_latelitude-val ((m <Apl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader relative_angle_nb-msg:obs_latelitude-val is deprecated.  Use relative_angle_nb-msg:obs_latelitude instead.")
  (obs_latelitude m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Apl>) ostream)
  "Serializes a message object of type '<Apl>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'obs_dist)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'obs_dist)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'obs_direct))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'goal_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'goal_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'obs_latelitude)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Apl>) istream)
  "Deserializes a message object of type '<Apl>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'obs_dist)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'obs_dist)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'obs_direct) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'goal_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'goal_y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'obs_latelitude) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Apl>)))
  "Returns string type for a message object of type '<Apl>"
  "relative_angle_nb/Apl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Apl)))
  "Returns string type for a message object of type 'Apl"
  "relative_angle_nb/Apl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Apl>)))
  "Returns md5sum for a message object of type '<Apl>"
  "9809d650616fd3c284b0b4b27cd2e9fe")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Apl)))
  "Returns md5sum for a message object of type 'Apl"
  "9809d650616fd3c284b0b4b27cd2e9fe")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Apl>)))
  "Returns full string definition for message of type '<Apl>"
  (cl:format cl:nil "uint16 obs_dist~%float32 obs_direct~%float32 goal_x~%float32 goal_y~%int32 obs_latelitude~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Apl)))
  "Returns full string definition for message of type 'Apl"
  (cl:format cl:nil "uint16 obs_dist~%float32 obs_direct~%float32 goal_x~%float32 goal_y~%int32 obs_latelitude~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Apl>))
  (cl:+ 0
     2
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Apl>))
  "Converts a ROS message object to a list"
  (cl:list 'Apl
    (cl:cons ':obs_dist (obs_dist msg))
    (cl:cons ':obs_direct (obs_direct msg))
    (cl:cons ':goal_x (goal_x msg))
    (cl:cons ':goal_y (goal_y msg))
    (cl:cons ':obs_latelitude (obs_latelitude msg))
))
