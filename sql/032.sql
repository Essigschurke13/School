SELECT manager_id, MIN(salary) FROM employees GROUP BY manager_id ORDER BY salary DESC, manager_id DESC;