import type { Metadata } from "next";
import { Geist, JetBrains_Mono } from "next/font/google";
import Link from "next/link";
import { BackToTop } from "@/components/BackToTop";
import { GitHubIcon } from "@/components/Icons";
import "./globals.css";

const geistSans = Geist({
  variable: "--font-geist-sans",
  subsets: ["latin"],
});

const jetbrainsMono = JetBrains_Mono({
  variable: "--font-geist-mono",
  subsets: ["latin"],
});

export const metadata: Metadata = {
  title: "MySTL Notes",
  description: "C++ STL 学习笔记",
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="zh-CN">
      <body
        className={`${geistSans.variable} ${jetbrainsMono.variable} antialiased`}
      >
        <header className="fixed top-0 left-0 right-0 z-50 bg-background/80 backdrop-blur-sm border-b border-border">
          <nav className="max-w-3xl mx-auto px-6 h-14 flex items-center justify-between">
            <Link href="/" className="font-semibold hover:text-accent">
              MySTL
            </Link>
            <a
              href="https://github.com/qiekn/mystl"
              target="_blank"
              rel="noopener noreferrer"
              className="flex items-center gap-2 text-sm text-muted hover:text-foreground transition-colors"
            >
              <GitHubIcon />
              <span>qiekn/mystl</span>
            </a>
          </nav>
        </header>
        <div className="pt-14">{children}</div>
        <BackToTop />
      </body>
    </html>
  );
}
