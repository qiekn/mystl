import fs from "fs";
import path from "path";
import matter from "gray-matter";

const contentDir = path.join(process.cwd(), "content");

export function getMDXList(subDir: string) {
  const dirPath = path.join(contentDir, subDir);

  // 如果目录不存在，返回空数组
  if (!fs.existsSync(dirPath)) return [];

  const files = fs.readdirSync(dirPath);

  return files
    .filter((file) => file.endsWith(".mdx"))
    .map((file) => {
      const filePath = path.join(dirPath, file);
      const source = fs.readFileSync(filePath, "utf8");
      const { data } = matter(source);

      return {
        slug: file.replace(".mdx", ""),
        ...data,
      };
    });
}

// 获取单篇文章详情（包含正文）
export function getMDXDetail(subDir: string, slug: string) {
  const filePath = path.join(contentDir, subDir, `${slug}.mdx`);
  const source = fs.readFileSync(filePath, "utf8");
  const { content, data } = matter(source);

  return {
    content, // MDX 正文
    metadata: data, // Frontmatter 属性
  };
}
