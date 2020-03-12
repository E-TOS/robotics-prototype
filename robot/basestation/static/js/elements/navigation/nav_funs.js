function Direction(p1_lat, p1_lon, p2_lat, p2_lon) {
	/*
	This is a function to calculate the direction from p1 (point1) to p2 (point2)
	Inputs are p1 and p2 latitudes and longitudes in degrees
	Output is the direction from p1 to p2 in degrees
	*/
	
	p1_lat = p1_lat * (Math.PI / 180);
	p1_lon = p1_lon * (Math.PI / 180);
	p2_lat = p2_lat * (Math.PI / 180);
	p2_lon = p2_lon * (Math.PI / 180);
	
	let delta_lat = p2_lat - p1_lat;
	let delta_lon = p2_lon - p1_lon;
	
	let X = Math.cos(p2_lat) * Math.sin(delta_lon);
	let Y = Math.cos(p1_lat) * Math.sin(p2_lat) - Math.sin(p1_lat) * Math.cos(p2_lat) * Math.cos(delta_lon);
	
	let Real_dir = Math.atan2(X,Y);
	Real_dir = Real_dir * (180 / Math.PI);
	
	if (Real_dir < 0) {
		Real_dir = Real_dir + 360;
    }
    
	return Real_dir;
}






function Distance(p1_lat, p1_lon, p2_lat, p2_lon) {
	/*
	This is a function to calculate the distance between p1 (point1) to p2 (point2)
	Inputs are p1 and p2 latitudes and longitudes in degrees
	Output is the distance between p1 and p2 in meters
	*/
	let R = 6371000;
	p1_lat = p1_lat * (Math.PI / 180);
	p1_lon = p1_lon * (Math.PI / 180);
	p2_lat = p2_lat * (Math.PI / 180);
	p2_lon = p2_lon * (Math.PI / 180);
	
	let delta_lat = p2_lat - p1_lat;
	let delta_lon = p2_lon - p1_lon;
	
	let A = ((Math.sin(delta_lat / 2)) ** 2) + Math.cos(p2_lat) * Math.cos(p1_lat) * ((Math.sin(delta_lon / 2)) ** 2);
	let C = 2 * Math.atan2(Math.sqrt(A) , Math.sqrt(1 - A));
	return (R * C);
}


function Turning(Dest_dir , heading) {
	/*
	This function is to determine the needed adjustment in direction to go to destination
	Input is the direction to destination and rover heading in degrees
	Output is the needed adjustment in degrees
	if the output is positive number , it means the rover needs to turn right
	if the output is negative number , it means the rover needs to turn left
	*/
	let shift = Dest_dir - heading;
	if (shift > 0 && shift < 180) {
		return shift;
    } else if (shift > 180 && shift < 360) {
		return (shift - 360);
    } else if (shift < 0 && shift > -180) {
		return shift;
    } else if (shift < -180 && shift > -360) {
		return (shift + 360);
    }
}

/*
BS = {'lat':0 , 'lon':0}
Rover = {'lat':0 , 'lon':0}

BS['lat'] = float(input("Enter BS latitude :"))
BS['lon'] = float(input("Enter BS longitude :"))
Rover['lat'] = float(input("Enter Rover latitude :"))
Rover['lon'] = float(input("Enter Rover longitude :"))

Dir = Direction(BS['lat'],BS['lon'],Rover['lat'],Rover['lon'])
Dis = Distance(BS['lat'],BS['lon'],Rover['lat'],Rover['lon'])

print('The direction is : {} degrees'.format(round(Dir)))
print('The Distance is : {} meters'.format(round(Dis)))
*/