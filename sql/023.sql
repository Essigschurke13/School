SELECT last_name, job_id, salary FROM employees WHERE (job_id == "IT_PROG" OR job_id == "SH_CLERK") AND (salary != 4500 AND salary != 10000 AND salary != 15000);