import Link from "next/link";
import { getMDXList } from "@/lib/mdx";

interface NoteItem {
  slug: string;
  title?: string;
  date?: string;
  order?: number;
  tags?: string[];
}

export default function Home() {
  const notes = getMDXList("notes") as NoteItem[];

  // Sort by order field
  const sortedNotes = notes.sort((a, b) => (a.order ?? 99) - (b.order ?? 99));

  return (
    <main className="min-h-screen px-6 py-16 md:px-12 lg:px-24">
      <div className="max-w-3xl mx-auto">
        {/* Hero */}
        <header className="mb-8">
          <h1 className="text-4xl font-bold mb-4">MySTL</h1>
          <p className="text-muted leading-relaxed">
            这是一个 C++ 学习项目，目标是从零实现一个简易版的 STL。
            通过亲手实现每个组件，深入理解模板元编程、内存管理、迭代器设计等核心概念。
          </p>
        </header>

        {/* Roadmap */}
        <section className="mb-16">
          <h2 className="text-2xl font-semibold mb-6">实现路线</h2>
          <div className="grid gap-4">
            <PhaseCard
              phase={1}
              title="基础设施"
              items={[
                "type_traits",
                "iterator",
                "util",
                "construct",
                "allocator",
              ]}
            />
            <PhaseCard
              phase={2}
              title="序列容器"
              items={["vector", "list", "deque"]}
            />
            <PhaseCard
              phase={3}
              title="容器适配器"
              items={["stack", "queue", "priority_queue"]}
            />
            <PhaseCard
              phase={4}
              title="关联容器"
              items={[
                "rb_tree",
                "set",
                "map",
                "hashtable",
                "unordered_set",
                "unordered_map",
              ]}
            />
            <PhaseCard
              phase={5}
              title="算法"
              items={["algobase", "algorithm"]}
            />
          </div>
        </section>

        {/* Notes List */}
        <section>
          <h2 className="text-2xl font-semibold mb-6">学习笔记</h2>
          {sortedNotes.length === 0 ? (
            <p className="text-muted">暂无笔记</p>
          ) : (
            <div className="space-y-4">
              {sortedNotes.map((note) => (
                <Link
                  key={note.slug}
                  href={`/notes/${note.slug}`}
                  className="block p-4 rounded-lg border border-border hover:border-accent transition-colors"
                >
                  <div className="flex items-start justify-between gap-4">
                    <div>
                      <h3 className="font-medium mb-1">
                        {note.title ?? note.slug}
                      </h3>
                      {note.tags && (
                        <div className="flex gap-2">
                          {note.tags.map((tag) => (
                            <span
                              key={tag}
                              className="text-xs px-2 py-0.5 rounded bg-accent-light text-accent"
                            >
                              {tag}
                            </span>
                          ))}
                        </div>
                      )}
                    </div>
                    {note.date && (
                      <span className="text-sm text-muted shrink-0">
                        {note.date}
                      </span>
                    )}
                  </div>
                </Link>
              ))}
            </div>
          )}
        </section>
      </div>
    </main>
  );
}

function PhaseCard({
  phase,
  title,
  items,
}: {
  phase: number;
  title: string;
  items: string[];
}) {
  return (
    <div className="p-4 rounded-lg border border-border">
      <div className="flex items-center gap-3 mb-2">
        <span className="w-7 h-7 rounded-full bg-muted/30 text-muted text-sm font-medium flex items-center justify-center">
          {phase}
        </span>
        <h3 className="font-medium">{title}</h3>
      </div>
      <div className="flex flex-wrap gap-2 ml-10">
        {items.map((item) => (
          <code
            key={item}
            className="text-xs px-2 py-1 rounded bg-accent-light text-accent"
          >
            {item}
          </code>
        ))}
      </div>
    </div>
  );
}
