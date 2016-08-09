
(cl:in-package :asdf)

(defsystem "PositionH-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "PH_RK" :depends-on ("_package_PH_RK"))
    (:file "_package_PH_RK" :depends-on ("_package"))
  ))