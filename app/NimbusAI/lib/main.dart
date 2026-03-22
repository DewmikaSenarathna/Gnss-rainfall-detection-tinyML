import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';

import 'screens/dashboard_screen.dart';
import 'utils/app_theme.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  if (Firebase.apps.isEmpty) {
    // On Android/iOS, use native Firebase config files.
    await Firebase.initializeApp();
  }

  runApp(const RainApp());
}

class RainApp extends StatelessWidget {
  const RainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'NimbusAI',
      theme: AppTheme.lightTheme,
      home: const DashboardScreen(),
    );
  }
}