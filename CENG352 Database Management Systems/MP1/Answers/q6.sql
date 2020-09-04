select 
	w.weekday_id, w.weekday_name, avg(fr.departure_delay + fr.arrival_delay) as avg_delay
from 
	flight_reports fr, weekdays w
where 
	fr.origin_city_name = 'San Francisco, CA' and 
	fr.dest_city_name = 'Boston, MA' and w.weekday_id = fr.weekday_id
group by 
	w.weekday_id
having 
	avg(fr.departure_delay + fr.arrival_delay) <= all 
													(select 
														avg(fr1.departure_delay + fr1.arrival_delay)
													 from 
													 	flight_reports fr1, weekdays w1
													 where 
													 	fr1.origin_city_name = 'San Francisco, CA' and 
														fr1.dest_city_name = 'Boston, MA' and w1.weekday_id = fr1.weekday_id
													 group by 
													 	w1.weekday_id);
