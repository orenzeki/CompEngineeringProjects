select 
	fr.plane_tail_number, (avg(fr.flight_distance*1.0/fr.flight_time)) as avg_speed
from 
	flight_reports fr
where 
	fr.plane_tail_number in (select 
								fr3.plane_tail_number
							 from 
							 	flight_reports fr3
							 where 
							 	fr3.weekday_id in (6, 7) and fr3."month" = 1 and fr3."year" = 2016 and fr3.is_cancelled = 0
							 except
							 select 
							 	fr2.plane_tail_number
							 from 
							 	flight_reports fr2
   							 where 
   							 	fr2.weekday_id not in (6, 7) and fr2."month" = 1 and 
   							 	fr2."year" = 2016 and fr2.is_cancelled = 0) and
								fr."month" = 1 and fr."year" = 2016 and fr.is_cancelled = 0
group by 
	fr.plane_tail_number
order by 
	avg_speed desc;
