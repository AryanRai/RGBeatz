// main.dart

import 'dart:ffi';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';


import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_core/firebase_core.dart';


import 'package:blobs/blobs.dart';
import 'package:glassmorphism/glassmorphism.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:liquid_progress_indicator/liquid_progress_indicator.dart';

import 'package:google_sign_in/google_sign_in.dart';
import 'package:flutter_signin_button/flutter_signin_button.dart';




void main() {
  WidgetsFlutterBinding.ensureInitialized();
  BlobController blobCtrl =  BlobController();
  BlobData blobData = blobCtrl.change();
  WidgetsFlutterBinding.ensureInitialized();

  //SystemChrome.setEnabledSystemUIOverlays([SystemUiOverlay.top]);
  SystemChrome.setSystemUIOverlayStyle(SystemUiOverlayStyle(
      statusBarColor: Colors.transparent,
      systemNavigationBarColor: Color(0xFF090036),
      systemNavigationBarDividerColor: Colors.transparent,


  ));

  runApp(App());




}

/*
GoogleSignIn _googleSignIn = GoogleSignIn(
  scopes: [
    'email',
    'https://www.googleapis.com/auth/contacts.readonly',
  ],
);

Future<void> _handleSignIn() async {
  try {
    final googleUser = await _googleSignIn.signIn();
  } catch (error) {
    print(error);
  }
}
*/

Future<UserCredential> signInWithGoogle() async {
  // Trigger the authentication flow
  final googleUser = await GoogleSignIn().signIn();

  // Obtain the auth details from the request
  final GoogleSignInAuthentication googleAuth = await googleUser!.authentication;

  // Create a new credential
  final credential = GoogleAuthProvider.credential(
    accessToken: googleAuth.accessToken,
    idToken: googleAuth.idToken,
  );

  // Once signed in, return the UserCredential
  return await FirebaseAuth.instance.signInWithCredential(credential);

}


class App extends StatefulWidget {
  // Create the initialization Future outside of `build`:
  @override
  _AppState createState() => _AppState();
}

class _AppState extends State<App> {
  /// The future is part of the state of our widget. We should not call `initializeApp`
  /// directly inside [build].
  final Future<FirebaseApp> _initialization = Firebase.initializeApp();


  @override


  Widget build(BuildContext context) {
    return FutureBuilder(
      // Initialize FlutterFire:
      future: _initialization,
      builder: (context, snapshot) {
        // Check for errors
        if (snapshot.hasError) {
          return errorfirebase();
        }

        // Once complete, show your application
        if (snapshot.connectionState == ConnectionState.done) {
          return MyApp();
        }

        // Otherwise, show something whilst waiting for initialization to complete
        return loading();
      },
    );
  }
}

class MyApp extends StatelessWidget {

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      // Remove the debug banner
      debugShowCheckedModeBanner: false,
      title: 'RGBeatz',
      home: HomePage(),
    );
  }
}

class HomePage extends StatelessWidget {



  @override

  Widget build(BuildContext context) {
    return Container(

      decoration: BoxDecoration(
          gradient: LinearGradient(
              begin: Alignment.topLeft,
              end: Alignment.bottomRight,
              colors: [Color(0xFF8459FF), Color(0xFF090036)])),
      child: (


          Scaffold(

        // By defaut, Scaffold background is white
        // Set its value to transparent
          backgroundColor: Colors.transparent,
          body: Center(
          child: new Stack(

          children: [

            //new Container(
            new Positioned(
                bottom: -100,
                right: 150,
                child: (


              Blob.animatedRandom(
                size:400,
                edgesCount:5,
                minGrowth:8,
                duration:  Duration(milliseconds:1000),
                styles:  BlobStyles(

                  fillType:  BlobFillType.fill,
                  gradient:  LinearGradient(colors: [Color(0xFF7754F4), Color(
                      0xFF8FB2CA)])
                      .createShader(Rect.fromLTRB(0, 0, 300, 300)),
                  strokeWidth:1,
                ),
                loop:  true,

                )


            )
            ),

            new Positioned(
                bottom: 400,
                left: 250,
                child: (


                    Blob.animatedRandom(
                      size:350,
                      edgesCount:5,
                      minGrowth:8,
                      duration:  Duration(milliseconds:1000),
                      styles:  BlobStyles(

                        fillType:  BlobFillType.fill,
                        gradient:  LinearGradient(colors: [Color(0xFF7754F4), Color(
                            0xFF8FB2CA)])
                            .createShader(Rect.fromLTRB(0, 0, 300, 300)),
                        strokeWidth:1,
                      ),
                      loop:  true,

                    )


                )
            ),

            new Positioned(
                bottom: 650,
                right: 270,
                child: (


                    Blob.animatedRandom(
                      size:300,
                      edgesCount:5,
                      minGrowth:8,
                      duration:  Duration(milliseconds:1000),
                      styles:  BlobStyles(

                        fillType:  BlobFillType.fill,
                        gradient:  LinearGradient(colors: [Color(0xFF7754F4), Color(
                            0xFF8FB2CA)])
                            .createShader(Rect.fromLTRB(0, 0, 300, 300)),
                        strokeWidth:1,
                      ),
                      loop:  true,

                    )


                )
            ),
              //new Container(

            SafeArea(
              child: Center(
                child: GlassmorphicContainer(
                    width: MediaQuery.of(context).size.width * 0.88,
                    height: MediaQuery.of(context).size.height * 0.5,
                    borderRadius: 20,
                    blur: 20,
                    alignment: Alignment.bottomCenter,
                    border: 1,
                    //border: 2,
                    linearGradient: LinearGradient(
                        begin: Alignment.topLeft,
                        end: Alignment.bottomRight,
                        colors: [
                          Color(0xFFffffff).withOpacity(0.06),
                          Color(0xFFFFFFFF).withOpacity(0.09),
                        ],
                        stops: [
                          0.1,
                          1,
                        ]),
                    borderGradient: LinearGradient(
                        colors: [
                          Colors.white.withOpacity(0.30),
                          Colors.white.withOpacity(0.15),
                          Colors.grey.withOpacity(0.2),
                          Colors.grey.withOpacity(0.30),

                        ],
                        begin: Alignment(0.35, -1.0),
                        end: Alignment.bottomRight,
                        stops: [0.0, 0.30, 0.31, 1.0]
                      // stops: []
                    ),
                    padding: EdgeInsets.symmetric(horizontal: 25.0, vertical: 25.0),

                    child: null),
              ),
            ),

            new Positioned(
              bottom: MediaQuery.of(context).size.height * 0.65,
              left: MediaQuery.of(context).size.width * 0.12,
              right: MediaQuery.of(context).size.width * 0.12,
              child: Center(
                  child: Text(
                    'LOGIN TO',
                    style: GoogleFonts.poppins(
                      textStyle: Theme.of(context).textTheme.headline4,
                      fontSize: 20,
                      color: Color(0x79ffff94),
                      letterSpacing: 7,
                      fontWeight: FontWeight.w300,
                      fontStyle: FontStyle.normal,
                    ),
                  )
              ),
            ),

            new Positioned(
              bottom: MediaQuery.of(context).size.height * 0.55,
              left: MediaQuery.of(context).size.width * 0.12,
              right: MediaQuery.of(context).size.width * 0.12,
              child: Center(
              child: Text(
                'RGBeatz',
                textAlign: TextAlign.center,
                style: GoogleFonts.poppins(
                  textStyle: Theme.of(context).textTheme.headline4,
                  //fontSize: 48,
                  color: Color(0xd0000000),
                  fontSize: 50,
                  fontWeight: FontWeight.w700,
                  fontStyle: FontStyle.normal,
                ),
              )
              ),
            ),

            new Positioned(
              bottom: MediaQuery.of(context).size.height * 0.39,
              left: MediaQuery.of(context).size.width * 0.17,
              right: MediaQuery.of(context).size.width * 0.17,
              child: Center(
                  child: Text(
                    'With a valid Google account to continue your RGB journey',
                    textAlign: TextAlign.center,
                    style: GoogleFonts.poppins(
                      textStyle: Theme.of(context).textTheme.headline4,
                      fontSize: 18,
                      letterSpacing: 1,
                      color: Color(0xb4f5f5f5),
                      fontWeight: FontWeight.w700,
                      fontStyle: FontStyle.normal,
                    ),
                  )
              ),
            ),

            new Positioned(
                bottom: MediaQuery.of(context).size.height * 0.29,
              left: 100,
              right: 100,
              child: Container(
              //height: 50,
                // width: 500,

              child: Center(

                child: SignInButton(

                  Buttons.GoogleDark,
                  text: "Sign in with Google",
                  onPressed: () {

                    signInWithGoogle();

                  },
                ))

          ))]
          )



            ),

          )
          )
      );

  }
}

class loading extends StatelessWidget {



  @override

  Widget build(BuildContext context) {
    return MaterialApp(
      // Remove the debug banner
      debugShowCheckedModeBanner: false,
      title: 'RGBeatz',
      home: Container(

        decoration: BoxDecoration(
            gradient: LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [Color(0xFF8459FF), Color(0xFF090036)])),
        child: (


            Scaffold(

              // By defaut, Scaffold background is white
              // Set its value to transparent
              backgroundColor: Colors.transparent,
              body: Center(
                  child: new Stack(

                      children: [

                        //new Container(


                        new Positioned(

                            child: LiquidLinearProgressIndicator(
                              value: 0.75, // Defaults to 0.5.
                              valueColor: AlwaysStoppedAnimation(Color(
                                  0xFF735DDE)), // Defaults to the current Theme's accentColor.
                              backgroundColor: Color(0xFFB0ADC6), // Defaults to the current Theme's backgroundColor.
                              borderColor: Color(0xFF6ADFEA),
                              borderWidth: 0,
                              borderRadius: 12.0,
                              direction: Axis.vertical, // The direction the liquid moves (Axis.vertical = bottom to top, Axis.horizontal = left to right). Defaults to Axis.horizontal.
                              center: Text("Loading..."),
                            )




                        ),


                      ]
                  )


              ),

            )
        )
    ));

  }
}


class errorfirebase extends StatelessWidget {



  @override

  Widget build(BuildContext context) {
    return MaterialApp(
      // Remove the debug banner
        debugShowCheckedModeBanner: false,
        title: 'RGBeatz',
        home: Container(

            decoration: BoxDecoration(
                gradient: LinearGradient(
                    begin: Alignment.topLeft,
                    end: Alignment.bottomRight,
                    colors: [Color(0xFF8459FF), Color(0xFF090036)])),
            child: (


                Scaffold(

                  // By defaut, Scaffold background is white
                  // Set its value to transparent
                  backgroundColor: Colors.transparent,
                  body: Center(
                      child: new Stack(

                          children: [

                            //new Container(


                            new Positioned(

                                child: LiquidLinearProgressIndicator(
                                  value: 0.75, // Defaults to 0.5.
                                  valueColor: AlwaysStoppedAnimation(Color(
                                      0xFFE7848D)), // Defaults to the current Theme's accentColor.
                                  backgroundColor: Color(0xFFB0ADC6), // Defaults to the current Theme's backgroundColor.
                                  borderColor: Color(0xFF6ADFEA),
                                  borderWidth: 0,
                                  borderRadius: 12.0,
                                  direction: Axis.vertical, // The direction the liquid moves (Axis.vertical = bottom to top, Axis.horizontal = left to right). Defaults to Axis.horizontal.
                                  center: Text("Error..."),
                                )




                            ),


                          ]
                      )


                  ),

                )
            )
        ));

  }
}