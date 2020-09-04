select 
	fr.plane_tail_number, (fr.airline_code) as first_owner, (fr2.airline_code) as second_owner
from 
	flight_reports fr, flight_reports fr2
where 
	fr.airline_code <> fr2.airline_code and 
	(fr.year < fr2.year or 
	(fr.year = fr2.year and fr.month < fr2.month) or 
	(fr.year = fr2.year and fr.month = fr2.month and fr.day < fr2.day)) and 
	fr.plane_tail_number = fr2.plane_tail_number
group by 
	fr.plane_tail_number, fr.airline_code, fr2.airline_code
order by 
	fr.plane_tail_number, first_owner, second_owner asc;
