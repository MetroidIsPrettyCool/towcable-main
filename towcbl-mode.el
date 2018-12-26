;;; TOWCBL-mode.el --- TOWCBL syntax highlighting

(defvar towcbl-font-lock-keywords)
(setq towcbl-font-lock-keywords
      '(("\".*\"" . font-lock-string-face)
	("[0-9]+i" . font-lock-constant-face)
	("!.*!" . font-lock-comment-face)
	("@\\|\+\\|\-\\|&" . font-lock-warning-face)
	("\\(\\(ADD\\|SUB\\|MUL\\|DIV\\|MOD\\|LOC\\)N\\)\\|RAND\\|PAST" . font-lock-function-name-face)
	("DIMN\\|SIZE\\|SETP\\|RULE\\|CMPX\\|BEGN\\|OUPT\\|CRPT\\|INPT\\|CALL\\|CALX\\|LOOP\\|LOTO\\|COND\\|ENDC\\|STRO\\|EXIT" . font-lock-builtin-face)
	("\\(E\\|N\\)Q\\|\\(G\\|L\\)\\(T\\|E\\)" . font-lock-keyword-face)))

;;;###autoload
(define-derived-mode towcbl-mode fundamental-mode
  "towcbl"
  "Major mode for towcbl source code."
  (setq font-lock-defaults '(towcbl-font-lock-keywords))
  )

;;;###autoload
(add-to-list 'auto-mode-alist '("\\.tow\\'" . towcbl-mode))

;; add the mode to the `features' list
(provide 'towcbl-mode)

;; Local Variables:
;; coding: utf-8
;; End:

;;; towcbl-mode.el ends here
