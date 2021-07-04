
(cl:in-package :asdf)

(defsystem "relative_angle_nb-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Apl" :depends-on ("_package_Apl"))
    (:file "_package_Apl" :depends-on ("_package"))
    (:file "Apl_kai" :depends-on ("_package_Apl_kai"))
    (:file "_package_Apl_kai" :depends-on ("_package"))
  ))