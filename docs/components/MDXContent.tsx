import { MDXRemote } from "next-mdx-remote/rsc";
import remarkGfm from "remark-gfm";
import { codeToHtml } from "shiki";

// Code block (```cpp ... ```)
async function CodeBlock({
  children,
  className,
}: {
  children?: string;
  className?: string;
}) {
  const lang = className?.replace("language-", "") || "text";
  const code = children?.trim() || "";

  const html = await codeToHtml(code, {
    lang,
    theme: "github-dark",
  });

  return <div dangerouslySetInnerHTML={{ __html: html }} />;
}

// Inline code (`code`)
function InlineCode({ children }: { children?: React.ReactNode }) {
  return (
    <code className="rounded px-1.5 py-0.5 bg-neutral-200 dark:bg-neutral-800 font-mono text-[0.875em]">
      {children}
    </code>
  );
}

// Pre wrapper - renders CodeBlock for code blocks
async function Pre({
  children,
}: {
  children?: React.ReactElement<{ children?: string; className?: string }>;
}) {
  const childProps = children?.props;
  const className = childProps?.className || "";
  const code = childProps?.children || "";

  return <CodeBlock className={className}>{code}</CodeBlock>;
}

// Code component - only handles inline code now
// Code blocks go through Pre -> CodeBlock
function Code({
  children,
  className,
}: {
  children?: React.ReactNode;
  className?: string;
}) {
  // If has className, it's inside a pre (code block) - let Pre handle it
  if (className) {
    return <code className={className}>{children}</code>;
  }
  // Otherwise it's inline code
  return <InlineCode>{children}</InlineCode>;
}

const components = {
  pre: Pre,
  code: Code,
  h2: ({ children }: { children?: React.ReactNode }) => {
    const id =
      typeof children === "string"
        ? children.toLowerCase().replace(/\s+/g, "-")
        : undefined;
    return <h2 id={id}>{children}</h2>;
  },
  h3: ({ children }: { children?: React.ReactNode }) => {
    const id =
      typeof children === "string"
        ? children.toLowerCase().replace(/\s+/g, "-")
        : undefined;
    return <h3 id={id}>{children}</h3>;
  },
};

export function MDXContent({ source }: { source: string }) {
  return (
    <MDXRemote
      source={source}
      components={components}
      options={{ mdxOptions: { remarkPlugins: [remarkGfm] } }}
    />
  );
}
