import { fileURLToPath, URL } from 'node:url'
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueDevTools from 'vite-plugin-vue-devtools'
import gzipPlugin from 'rollup-plugin-gzip'
import cleanPlugin from 'rollup-plugin-delete'
import svgLoader from 'vite-svg-loader'
import tailwindcss from '@tailwindcss/vite'

// https://vite.dev/config/
export default defineConfig({
  plugins: [
    vue(),
    vueDevTools(),
    svgLoader(),
    tailwindcss(),
    gzipPlugin({
      customCompression: (content: string | Buffer) => Buffer.from(Bun.gzipSync(content)),
    }),
    cleanPlugin({
      targets: ['../data/public/**/*.html', '../data/public/**/*.js', '../data/public/**/*.css'],
      force: true,
      runOnce: true,
      hook: 'closeBundle',
      verbose: true,
    }),
  ],
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
  },
})
