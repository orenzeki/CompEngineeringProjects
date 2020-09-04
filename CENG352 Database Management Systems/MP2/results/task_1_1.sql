SELECT p.pub_type, COUNT(*) AS num_pub 
FROM Pub p 
GROUP BY p.pub_type 
ORDER BY num_pub DESC;

