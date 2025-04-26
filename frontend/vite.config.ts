import { fileURLToPath, URL } from 'node:url'
import { readdirSync, readFileSync, unlinkSync, writeFileSync, lstatSync } from 'node:fs'
import { resolve, extname } from 'node:path'
import { gzipSync } from 'node:zlib'
import { styleText } from 'node:util'
import { defineConfig, Plugin } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueDevTools from 'vite-plugin-vue-devtools'
import svgLoader from 'vite-svg-loader'
import tailwindcss from '@tailwindcss/vite'

const viteGzipPlugin = (options?: { log?: boolean }): Plugin => {
  const acceptedExt = ['.html', '.css', '.js', '.ico']
  const niceBytes = (byteLength: string) => {
    const units = ['b', 'kB', 'mB']
    let l = 0,
      n = parseInt(byteLength, 10) || 0

    while (n >= 1024 && ++l) {
      n = n / 1024
    }

    return n.toFixed(n < 10 && l > 0 ? 1 : 0) + ' ' + units[l]
  }

  return {
    name: 'vite-compress-plugin',
    apply: 'build',
    enforce: 'post',
    async closeBundle() {
      if (options?.log) {
        console.info(styleText('blue', 'Start compressing'))
      }
      const timeStart = Date.now()
      const outputLineGap = 4
      let maxLine = 0
      const results: [string, string][] = []

      const files = readdirSync(this.environment.config.build.outDir, { recursive: true })
      files.forEach((filePath) => {
        // Check file extension
        if (!acceptedExt.includes(extname(String(filePath)))) return
        const absoluteFilePath = resolve(
          process.cwd(),
          this.environment.config.build.outDir,
          filePath as string,
        )
        // Check is file
        if (lstatSync(absoluteFilePath).isDirectory()) return
        const file = readFileSync(absoluteFilePath)
        const compressedFile = gzipSync(file)
        unlinkSync(absoluteFilePath)
        writeFileSync(absoluteFilePath + '.gz', compressedFile)

        if (options?.log) {
          const outputFileName = this.environment.config.build.outDir + filePath + '.gz'
          const outputFileSize = niceBytes(compressedFile.byteLength.toString())
          const lineSize = outputFileName.length + outputFileSize.length
          maxLine = lineSize > maxLine ? lineSize : maxLine
          results.push([outputFileName, outputFileSize])
        }
      })
      if (options?.log) {
        console.info(
          results.reduce((output, [fileName, fileSize], index) => {
            return (
              output +
              (index ? '\n' : '') +
              fileName.padEnd(maxLine + outputLineGap - fileSize.length) +
              fileSize
            )
          }, ''),
        )

        console.info(styleText('green', `✓ Compressed in ${Date.now() - timeStart}ms`))
      }
    },
  }
}

// https://vite.dev/config/
export default defineConfig({
  plugins: [vue(), vueDevTools(), svgLoader(), tailwindcss(), viteGzipPlugin()],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url)),
    },
  },
  build: {
    outDir: '../data/public/',
    emptyOutDir: true,
    rollupOptions: {
      output: {
        chunkFileNames: (chunkInfo) => `assets/${chunkInfo.name.split('.')[0]}-[hash].js`,
      },
    },
    reportCompressedSize: true,
  },
})
