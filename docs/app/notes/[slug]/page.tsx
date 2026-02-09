import Link from "next/link";
import { getMDXList, getMDXDetail } from "@/lib/mdx";
import { MDXContent } from "@/components/MDXContent";
import { TableOfContents } from "@/components/TOC";

interface Params {
  slug: string;
}

interface Metadata {
  title?: string;
  date?: string;
  tags?: string[];
  toc?: boolean;
}

export async function generateStaticParams() {
  const notes = getMDXList("notes");
  return notes.map((note) => ({
    slug: note.slug,
  }));
}

export default async function NotePage({
  params,
}: {
  params: Promise<Params>;
}) {
  const { slug } = await params;
  const { content, metadata } = getMDXDetail("notes", slug);
  const meta = metadata as Metadata;
  const tocEnabled = meta.toc !== false; // default true

  return (
    <main className="min-h-screen px-6 py-16 md:px-12 lg:px-24">
      <TableOfContents enabled={tocEnabled} />
      <div className="max-w-3xl mx-auto">
        {/* Back link */}
        <Link
          href="/"
          className="inline-flex items-center gap-1 text-sm text-muted hover:text-accent mb-8"
        >
          <svg
            className="w-4 h-4"
            fill="none"
            stroke="currentColor"
            viewBox="0 0 24 24"
          >
            <path
              strokeLinecap="round"
              strokeLinejoin="round"
              strokeWidth={2}
              d="M15 19l-7-7 7-7"
            />
          </svg>
          返回首页
        </Link>

        {/* Article header */}
        <header className="mb-10">
          <h1 className="text-3xl font-bold mb-4">{meta.title ?? slug}</h1>
          <div className="flex items-center gap-4 text-sm text-muted">
            {meta.date && <span>{meta.date}</span>}
            {meta.tags && (
              <div className="flex gap-2">
                {meta.tags.map((tag) => (
                  <span
                    key={tag}
                    className="px-2 py-0.5 rounded bg-accent-light text-accent"
                  >
                    {tag}
                  </span>
                ))}
              </div>
            )}
          </div>
        </header>

        {/* MDX Content */}
        <article className="prose prose-neutral dark:prose-invert max-w-none">
          <MDXContent source={content} />
        </article>
      </div>
    </main>
  );
}
