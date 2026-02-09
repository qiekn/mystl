"use client";

import { useEffect, useState } from "react";

interface TocItem {
  id: string;
  text: string;
  level: number;
}

interface TableOfContentsProps {
  enabled?: boolean;
}

export function TableOfContents({ enabled = true }: TableOfContentsProps) {
  const [headings, setHeadings] = useState<TocItem[]>([]);
  const [activeId, setActiveId] = useState<string>("");
  const [isHovered, setIsHovered] = useState(false);

  useEffect(() => {
    if (!enabled) return;

    const article = document.querySelector("article");
    if (!article) return;

    const elements = article.querySelectorAll("h2, h3");
    const items: TocItem[] = [];

    elements.forEach((el, index) => {
      const id = el.id || `heading-${index}`;
      if (!el.id) el.id = id;

      items.push({
        id,
        text: el.textContent || "",
        level: el.tagName === "H2" ? 2 : 3,
      });
    });

    setHeadings(items);

    const observer = new IntersectionObserver(
      (entries) => {
        entries.forEach((entry) => {
          if (entry.isIntersecting) {
            setActiveId(entry.target.id);
          }
        });
      },
      { rootMargin: "-80px 0px -80% 0px" },
    );

    elements.forEach((el) => observer.observe(el));

    return () => observer.disconnect();
  }, [enabled]);

  const handleClick = (e: React.MouseEvent<HTMLAnchorElement>, id: string) => {
    e.preventDefault();
    const element = document.getElementById(id);
    if (element) {
      const offset = 80;
      const top = element.getBoundingClientRect().top + window.scrollY - offset;
      window.scrollTo({ top, behavior: "smooth" });
    }
  };

  if (!enabled || headings.length === 0) return null;

  return (
    <nav
      className="hidden lg:block fixed left-8 top-[20%] z-50"
      onMouseEnter={() => setIsHovered(true)}
      onMouseLeave={() => setIsHovered(false)}
    >
      {/* Collapsed: small bars */}
      <div
        className={`flex flex-col gap-2.5 transition-all duration-200 ${
          isHovered ? "opacity-0 scale-95" : "opacity-100 scale-100"
        }`}
      >
        {headings.map((heading, index) => (
          <div
            key={index}
            className={`h-0.5 transition-colors ${
              heading.level === 3 ? "ml-2.5 w-4" : "w-4"
            } ${activeId === heading.id ? "bg-accent" : "bg-muted"}`}
          />
        ))}
      </div>

      {/* Expanded: full TOC */}
      <div
        className={`absolute left-0 top-0 min-w-36 bg-background/95 backdrop-blur rounded-lg border border-border p-3 transition-all duration-200 origin-top-left ${
          isHovered
            ? "opacity-100 scale-100"
            : "opacity-0 scale-95 pointer-events-none"
        }`}
      >
        <ul className="text-sm whitespace-nowrap">
          {headings.map((heading) => (
            <li key={heading.id}>
              <a
                href={`#${heading.id}`}
                onClick={(e) => handleClick(e, heading.id)}
                className={`block py-1 px-2 rounded transition-colors ${
                  heading.level === 3 ? "pl-4" : ""
                } ${
                  activeId === heading.id
                    ? "text-accent"
                    : "text-muted hover:text-foreground"
                }`}
              >
                {heading.text}
              </a>
            </li>
          ))}
        </ul>
      </div>
    </nav>
  );
}
