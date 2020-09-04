select 
	ac2.airline_name, fr2."year", count(*) as total_num_flights, cancelled.cancelled_flights
from 
	(select 
		b.airline_code
     from 
     	(select 
     		a.airline_code, a.year, avg(a.cnt)
		 from 
		 	(select 
		 		fr.airline_code, fr."day", fr."month", fr."year", count(*) as cnt
			 from 
			 	flight_reports fr
			 group by 
			 	fr.airline_code, fr."day", fr."month", fr."year") as a
		  group by 
		  	a.airline_code, a.year
		  having 
		  	avg(a.cnt) >2000) as b
	  group by 
	  	b.airline_code
	  having 
	  	count(*)=4) as fnl, flight_reports fr2, 
	 (select 
	 	fr.airline_code, fr."year", (count(*)) as cancelled_flights
	  from 
	  	flight_reports fr
	  where 
	  	fr.is_cancelled = 1
	  group by 
	  	fr.airline_code, fr."year") as cancelled, airline_codes ac2
where 
	fnl.airline_code = fr2.airline_code and cancelled.airline_code = fr2.airline_code and 
	cancelled.year = fr2."year" and ac2.airline_code = fr2.airline_code
group by 
	ac2.airline_name, fr2."year", cancelled.cancelled_flights
order by 
	ac2.airline_name asc;





