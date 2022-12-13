All the steps for Project E were completed, a process table was implemented, execute program and handle timer were modified to
run to programs simultaneously, and killProcess was also implemented to be able to stop a chosen program. To verify that it works
just open java -jar simulator.jar, boot the diskc.img and you should then be given the SHELL>: prompt. At that point type "exec number"
and then immediately type "exec letter" and you should see that both number and letter are running. Then to stop one of the process
type kill 1 (number) or kill 2 (letter) and if you want to complete stop the shell (why would you?) type kill 0.
