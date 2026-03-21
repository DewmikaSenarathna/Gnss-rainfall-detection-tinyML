import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

class AppTheme {
  AppTheme._();

  static const Color _primary = Color(0xFF0D486D);
  static const Color _secondary = Color(0xFF167A88);
  static const Color _surface = Color(0xFFF2F6FA);
  static const Color _text = Color(0xFF1F2A37);

  static ThemeData get lightTheme {
    final base = ThemeData(
      useMaterial3: true,
      colorScheme: ColorScheme.fromSeed(
        seedColor: _primary,
        primary: _primary,
        secondary: _secondary,
        surface: _surface,
      ),
      scaffoldBackgroundColor: _surface,
    );

    return base.copyWith(
      textTheme: GoogleFonts.sourceSerif4TextTheme(base.textTheme).copyWith(
        titleLarge: GoogleFonts.sourceSerif4(
          fontSize: 24,
          fontWeight: FontWeight.w700,
          color: _text,
        ),
        titleMedium: GoogleFonts.sourceSerif4(
          fontSize: 18,
          fontWeight: FontWeight.w600,
          color: _text,
        ),
        bodyMedium: GoogleFonts.sourceSans3(
          fontSize: 15,
          color: _text,
        ),
        bodyLarge: GoogleFonts.sourceSans3(
          fontSize: 17,
          color: _text,
        ),
      ),
      appBarTheme: AppBarTheme(
        backgroundColor: Colors.transparent,
        elevation: 0,
        centerTitle: false,
        foregroundColor: _text,
        titleTextStyle: GoogleFonts.sourceSerif4(
          fontSize: 24,
          fontWeight: FontWeight.w700,
          color: _text,
        ),
      ),
      cardTheme: CardThemeData(
        color: const Color(0xFFFCFDFE),
        margin: EdgeInsets.zero,
        elevation: 1,
        shadowColor: const Color(0xFF0D486D).withValues(alpha: 0.08),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(20),
          side: const BorderSide(color: Color(0xFFE4EBF3)),
        ),
      ),
      floatingActionButtonTheme: const FloatingActionButtonThemeData(
        backgroundColor: _primary,
        foregroundColor: Colors.white,
      ),
      filledButtonTheme: FilledButtonThemeData(
        style: FilledButton.styleFrom(
          backgroundColor: _primary,
          foregroundColor: Colors.white,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(14),
          ),
          padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 11),
        ),
      ),
    );
  }
}
