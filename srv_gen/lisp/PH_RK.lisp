; Auto-generated. Do not edit!


(cl:in-package PositionH-srv)


;//! \htmlinclude PH_RK-request.msg.html

(cl:defclass <PH_RK-request> (roslisp-msg-protocol:ros-message)
  ((kill
    :reader kill
    :initarg :kill
    :type cl:integer
    :initform 0)
   (run
    :reader run
    :initarg :run
    :type cl:integer
    :initform 0))
)

(cl:defclass PH_RK-request (<PH_RK-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PH_RK-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PH_RK-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name PositionH-srv:<PH_RK-request> is deprecated: use PositionH-srv:PH_RK-request instead.")))

(cl:ensure-generic-function 'kill-val :lambda-list '(m))
(cl:defmethod kill-val ((m <PH_RK-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader PositionH-srv:kill-val is deprecated.  Use PositionH-srv:kill instead.")
  (kill m))

(cl:ensure-generic-function 'run-val :lambda-list '(m))
(cl:defmethod run-val ((m <PH_RK-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader PositionH-srv:run-val is deprecated.  Use PositionH-srv:run instead.")
  (run m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PH_RK-request>) ostream)
  "Serializes a message object of type '<PH_RK-request>"
  (cl:let* ((signed (cl:slot-value msg 'kill)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'run)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PH_RK-request>) istream)
  "Deserializes a message object of type '<PH_RK-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'kill) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'run) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PH_RK-request>)))
  "Returns string type for a service object of type '<PH_RK-request>"
  "PositionH/PH_RKRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PH_RK-request)))
  "Returns string type for a service object of type 'PH_RK-request"
  "PositionH/PH_RKRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PH_RK-request>)))
  "Returns md5sum for a message object of type '<PH_RK-request>"
  "6865b3b355b3986f0576674e606efbad")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PH_RK-request)))
  "Returns md5sum for a message object of type 'PH_RK-request"
  "6865b3b355b3986f0576674e606efbad")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PH_RK-request>)))
  "Returns full string definition for message of type '<PH_RK-request>"
  (cl:format cl:nil "int32 kill~%int32 run~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PH_RK-request)))
  "Returns full string definition for message of type 'PH_RK-request"
  (cl:format cl:nil "int32 kill~%int32 run~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PH_RK-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PH_RK-request>))
  "Converts a ROS message object to a list"
  (cl:list 'PH_RK-request
    (cl:cons ':kill (kill msg))
    (cl:cons ':run (run msg))
))
;//! \htmlinclude PH_RK-response.msg.html

(cl:defclass <PH_RK-response> (roslisp-msg-protocol:ros-message)
  ((done
    :reader done
    :initarg :done
    :type cl:integer
    :initform 0))
)

(cl:defclass PH_RK-response (<PH_RK-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PH_RK-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PH_RK-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name PositionH-srv:<PH_RK-response> is deprecated: use PositionH-srv:PH_RK-response instead.")))

(cl:ensure-generic-function 'done-val :lambda-list '(m))
(cl:defmethod done-val ((m <PH_RK-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader PositionH-srv:done-val is deprecated.  Use PositionH-srv:done instead.")
  (done m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PH_RK-response>) ostream)
  "Serializes a message object of type '<PH_RK-response>"
  (cl:let* ((signed (cl:slot-value msg 'done)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PH_RK-response>) istream)
  "Deserializes a message object of type '<PH_RK-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'done) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PH_RK-response>)))
  "Returns string type for a service object of type '<PH_RK-response>"
  "PositionH/PH_RKResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PH_RK-response)))
  "Returns string type for a service object of type 'PH_RK-response"
  "PositionH/PH_RKResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PH_RK-response>)))
  "Returns md5sum for a message object of type '<PH_RK-response>"
  "6865b3b355b3986f0576674e606efbad")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PH_RK-response)))
  "Returns md5sum for a message object of type 'PH_RK-response"
  "6865b3b355b3986f0576674e606efbad")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PH_RK-response>)))
  "Returns full string definition for message of type '<PH_RK-response>"
  (cl:format cl:nil "int32 done~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PH_RK-response)))
  "Returns full string definition for message of type 'PH_RK-response"
  (cl:format cl:nil "int32 done~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PH_RK-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PH_RK-response>))
  "Converts a ROS message object to a list"
  (cl:list 'PH_RK-response
    (cl:cons ':done (done msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'PH_RK)))
  'PH_RK-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'PH_RK)))
  'PH_RK-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PH_RK)))
  "Returns string type for a service object of type '<PH_RK>"
  "PositionH/PH_RK")