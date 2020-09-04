select 
	ac.airline_name, count(*)
from 
	flight_reports fr, airline_codes ac
where 
	fr.plane_tail_number in (select 
								fr1.plane_tail_number
							 from 
							 	flight_reports fr1
							 where 
							 	fr1.dest_city_name like '%TX'
							 except
							 select 
							 	fr2.plane_tail_number
							 from 
							 	flight_reports fr2
							 where 
							 	fr2.dest_city_name not like '%TX') and 
								fr.is_cancelled = 0 and ac.airline_code = fr.airline_code
group by 
	ac.airline_name
order by 
	ac.airline_name asc;

