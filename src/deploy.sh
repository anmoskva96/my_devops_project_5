#!/bin/bash

scp /var/lib/gitlab-runner/builds/pmdXudXG/0/students/DO6_CICD.ID_356283/lemuelge_student.21_school.ru/DO6_CICD-2/src/cat/s21_cat root@10.10.0.1:/usr/local/bin/
scp /var/lib/gitlab-runner/builds/pmdXudXG/0/students/DO6_CICD.ID_356283/lemuelge_student.21_school.ru/DO6_CICD-2/src/grep/s21_grep root@10.10.0.1:/usr/local/bin/

ssh root@10.10.0.1 ls /usr/local/bin