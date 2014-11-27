;******************************************************
;*									*
;*	Initial Facts						*
;*									*
;******************************************************

(deffacts Initial-state
 (not_stopped)
 (score (home 0)(visitor 0))
 (not_initialized)
)

(defrule init-game
 ?f <- (not_initialized)
 =>
 (retract ?f)
 (init)
 (sendall stop)
 (sendall givectrl)
 ;(mysockOn "132.248.161.4" 8000 9000)
 (play)
 (sendall go)
 )
