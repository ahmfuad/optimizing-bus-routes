from geopy.geocoders import Nominatim
import folium
from tqdm import tqdm

# Function to get coordinates for a place
def get_coordinates(place):
    geolocator = Nominatim(user_agent="place_mapper")
    location = geolocator.geocode(place)
    #print(f"This is found -> {place}")
    if location:
        return (location.latitude, location.longitude)
    else:
        #print(f"This -> {place} cannot be found")
        return None

# Read the places from a .txt file and append ", Dhaka, Bangladesh"
places = []
with open('unique-bus-stops.txt', 'r') as file:
    for line in file:
        place = line.strip() + ", Dhaka, Bangladesh"  # Append suffix
        places.append(place)

# Get coordinates for each place
coordinates = []
print("Fetching coordinates for places...")
for place in tqdm(places, desc="Progress", unit="place"):
    coords = get_coordinates(place)
    if coords:
        coordinates.append((place, coords))

# Create a map centered at Dhaka
dhaka_center = [23.8103, 90.4125]  # Approximate center of Dhaka
mymap = folium.Map(location=dhaka_center, zoom_start=12)

# Add markers to the map
print("Fetching done...\nMarking coords in the map...")
for place, coord in coordinates:
    folium.Marker(location=coord, popup=place).add_to(mymap)

# Save the map as an HTML file
mymap.save("places_map.html")

print("Map generated and saved as 'places_map.html'")