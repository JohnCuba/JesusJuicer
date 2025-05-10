import { ref } from 'vue'

type useUploadFileArgs = {
  url?: string
}

export const useUploadFile = ({ url = '/' }: useUploadFileArgs) => {
  const progress = ref<number>(0)

  const resetProgress = () => (progress.value = 0)

  const upload = async (data: File) => {
    const xhr = new XMLHttpRequest()
    xhr.open('POST', url, true)

    xhr.onloadend = resetProgress
    xhr.onload = resetProgress
    xhr.onabort = resetProgress
    xhr.onerror = resetProgress

    xhr.upload.onprogress = (ev) => {
      progress.value = Math.trunc((ev.loaded / ev.total) * 100)
    }

    const formData = new FormData()
    const blob = new Blob([data], { type: 'application/octet-stream' })
    formData.append('file', blob, data.name)

    xhr.send(formData)
  }

  return {
    upload,
    progress,
  }
}
