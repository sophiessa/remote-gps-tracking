import 'dart:async';
import 'dart:typed_data';
import 'dart:ui' as ui;
import 'package:flutter/services.dart';
import 'package:location/location.dart';
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'package:socket_io_client/socket_io_client.dart' as IO;

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      title: 'App Title',
      home: MapPage(),
    );
  }
}

class MapPage extends StatefulWidget {
  const MapPage({Key? key}) : super(key: key);

  @override
  State<MapPage> createState() => MapPageState();
}

class MapPageState extends State<MapPage> {
  late IO.Socket socket;

  String title = "Header";

  late Map<MarkerId, Marker> _markers;

  Completer<GoogleMapController> _controller = Completer();


  late StreamSubscription _locationSubscription;

  final Location _locationtracker = Location();


  late Marker marker;
  late Circle circle;

  static const CameraPosition initialLocation = CameraPosition(
    target: LatLng(42.87,74.7311),
    zoom: 14,
  );

  @override
  void initState() {
    super.initState();

    _markers = <MarkerId, Marker>{};
    _markers.clear();
    initSocket();
  }

  Future<void> initSocket() async {

    try {

      socket = IO.io("SERVER_IP:SERVER_PORT", <String, dynamic>{
        'transports': ['websocket'],
        'autoConnect': true,
      });


      socket.connect();

      socket.on("message", (data) async {
        double lat = double.parse(data['lattitude']);
        double lng = double.parse(data['longitude']);
        double latitude = ((lat/100).floor()+(lat%100)/60);
        double longitude = ((lng/100).floor()+(lng%100)/60);

        String time = data['time'];
        String hour = time.substring(0, 2);
        String minute = time.substring(2, 4);
        String second = time.substring(4, 6);
        String bus = data['bus'];
        String speed = data['altitude'];

        title = "Bus # $bus| Time - $hour:$minute:$second";

        final Uint8List? imageData = await getMarker();
        var image = await BitmapDescriptor.fromAssetImage(const ImageConfiguration(), "assets/bus.png");

        Marker marker = Marker(
            markerId: MarkerId("ID"),
            icon: BitmapDescriptor.fromBytes(imageData!),
            position: LatLng(latitude, longitude),

        );

        setState(() {
          _markers[const MarkerId("ID")] = marker;
        });

      });
    } catch(e){

      title = e.toString();

    }
  }


  Future<Uint8List?> getMarker() async {
    ByteData data = await rootBundle.load("assets/bus.png");
    ui.Codec codec = await ui.instantiateImageCodec(
        data.buffer.asUint8List(), targetWidth: 200);
    ui.FrameInfo fi = await codec.getNextFrame();

    return (await fi.image.toByteData(format: ui.ImageByteFormat.png))?.buffer
        .asUint8List();
  }


  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(title),
      ),
      body: GoogleMap(

        mapType: MapType.normal,
        initialCameraPosition: initialLocation,
        onMapCreated: (GoogleMapController controller) {
          _controller.complete(controller);
        },
        markers: Set<Marker>.of(_markers.values),
      ),
    );
  }

}