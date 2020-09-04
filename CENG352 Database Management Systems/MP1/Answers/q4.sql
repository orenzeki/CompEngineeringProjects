select 
	*
from 
	(select 
		a.airline_name
	 from 
		(select 
			ac.airline_name, fr.dest_city_name
		 from 
			flight_reports fr, airline_codes ac
		 where 
			fr.airline_code = ac.airline_code and fr.is_cancelled = 0 and 
			fr.dest_city_name in ('Boston, MA', 'New York, NY', 'Portland, ME', 'Washington, DC', 'Philadelphia, PA') and 
			fr."year" in (2018, 2019)
	  	 group by 
	  	 	ac.airline_name, fr.dest_city_name) as a
	  group by 
	  	a.airline_name
	  having 
	  	count(a.airline_name) >= 5
	  except
	  select 
	  	a.airline_name
	  from 
	  	(select 
	  		ac.airline_name, fr.dest_city_name
	   	 from 
	   		flight_reports fr, airline_codes ac
	   	 where 
	   		fr.airline_code = ac.airline_code and fr.is_cancelled = 0 and 
	   		fr.dest_city_name in ('Boston, MA', 'New York, NY', 'Portland, ME', 'Washington, DC', 'Philadelphia, PA') and 
	   		fr."year" in (2016, 2017)
	   	 group by 
	   	 	ac.airline_name, fr.dest_city_name) as a
	  group by 
	  	a.airline_name
	  having 
	  	count(a.airline_name) >= 5) as c
order by 
	c.airline_name;
