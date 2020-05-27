;; Auto-generated. Do not edit!


(when (boundp 'serial_msgs::serial)
  (if (not (find-package "SERIAL_MSGS"))
    (make-package "SERIAL_MSGS"))
  (shadow 'serial (find-package "SERIAL_MSGS")))
(unless (find-package "SERIAL_MSGS::SERIAL")
  (make-package "SERIAL_MSGS::SERIAL"))

(in-package "ROS")
;;//! \htmlinclude serial.msg.html


(defclass serial_msgs::serial
  :super ros::object
  :slots (_serial ))

(defmethod serial_msgs::serial
  (:init
   (&key
    ((:serial __serial) (make-array 0 :initial-element 0 :element-type :char))
    )
   (send-super :init)
   (setq _serial __serial)
   self)
  (:serial
   (&optional __serial)
   (if __serial (setq _serial __serial)) _serial)
  (:serialization-length
   ()
   (+
    ;; uint8[] _serial
    (* 1    (length _serial)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8[] _serial
     (write-long (length _serial) s)
     (princ _serial s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8[] _serial
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _serial (make-array n :element-type :char))
     (replace _serial buf :start2 ptr-) (incf ptr- n))
   ;;
   self)
  )

(setf (get serial_msgs::serial :md5sum-) "91402c4025b67ddb569b7ca2fb02ac79")
(setf (get serial_msgs::serial :datatype-) "serial_msgs/serial")
(setf (get serial_msgs::serial :definition-)
      "uint8[] serial

")



(provide :serial_msgs/serial "91402c4025b67ddb569b7ca2fb02ac79")


