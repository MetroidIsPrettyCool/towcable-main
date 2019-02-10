;;; TOWCBL-mode.el --- TOWCBL syntax highlighting

(defvar towcbl-font-lock-keywords)
(setq towcbl-font-lock-keywords
      '(
	("[0-9]+i" . font-lock-constant-face)
	("@\\|\+\\|\-\\|&" . font-lock-warning-face)
	("\\(\\(ADD\\|SUB\\|MUL\\|DIV\\|MOD\\|LOC\\)N\\)\\|RAND\\|PAST\\|ATLO\\|RVAL" . font-lock-function-name-face)
	("DIMN\\|SIZE\\|SETP\\|RULE\\|CMPX\\|BEGN\\|OUPT\\|CRPT\\|INPT\\|CALL\\|CALX\\|LOOP\\|LOTO\\|COND\\|ENDC\\|STRO\\|EXIT\\|REGI\\|SETR\\|CLRS" . font-lock-builtin-face)
	("\\(E\\|N\\)Q\\|\\(G\\|L\\)\\(T\\|E\\)" . font-lock-keyword-face)))

(defvar towcbl-mode-syntax-table nil "Syntax table for `towcbl-mode'.")

(setq towcbl-mode-syntax-table
      (let ( (synTable (make-syntax-table)))
        (modify-syntax-entry ?! "!" synTable)
        synTable))

;;;###autoload
(define-derived-mode towcbl-mode prog-mode "towcbl"
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
