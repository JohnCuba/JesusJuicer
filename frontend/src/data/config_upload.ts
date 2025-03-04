import { ref } from 'vue'

type useUploadFileArgs = {
  url?: string
}

export const useUploadFile = ({ url = '/' }: useUploadFileArgs) => {
  const progress = ref<number>(0)

  const upload = async (data: File) => {
    const xhr = new XMLHttpRequest()
    xhr.open('POST', url, true)

    xhr.onload = () => {
      progress.value = 0
    }

    xhr.upload.onprogress = (ev) => {
      progress.value = ev.loaded / ev.total
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
