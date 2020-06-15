format long g

V = csvread("6_070620231601.csv");
#V = csvread("5_070620230921.csv");
#V = csvread("4_070620230242.csv");
#V = csvread("7_070620232023.csv");

lat = V(2:end, 3); lon = V(2:end, 4); h = V(2:end, 5);

[x, y, z] = geodetic2ecef("WGS84", lat, lon, h);
moy = mean([x y z], 1);
diff = [x, y, z] - moy;
distance = zeros(length(diff), 1);
for i = 1:length(diff)
  distance(i) = norm(diff(i), 2);
endfor
variance = var(distance)
ectype = std(distance)
points = length(diff)

#[x, y, z] .- moy

#distance([lat(1), lon(1)], [lat(2), lon(2)])

#norm([xa,ya,za]- [xb,yb,zb], 2)



#printf('%f, %f, %f\n', xa, ya, za)
#printf('%f, %f, %f\n', xb, yb, zb)