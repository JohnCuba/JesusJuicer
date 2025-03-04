/* eslint-disable */
/* tslint:disable */
/*
 * ---------------------------------------------------------------
 * ## THIS FILE WAS GENERATED VIA SWAGGER-TYPESCRIPT-API        ##
 * ##                                                           ##
 * ## AUTHOR: acacode                                           ##
 * ## SOURCE: https://github.com/acacode/swagger-typescript-api ##
 * ---------------------------------------------------------------
 */

export type WifiMode = '0' | '1' | '2' | '3'

/** A simplified representation of a wifiCredentials, typically used in list views. */
export interface WifiCredentials {
  ssid: string
  password?: string
}

export interface WifiCredentialsEdit {
  to?: number
  ssid: string
  index: number
  password?: string
}

export type QueryParamsType = Record<string | number, any>
export type ResponseFormat = keyof Omit<Body, 'body' | 'bodyUsed'>

export interface FullRequestParams extends Omit<RequestInit, 'body'> {
  /** set parameter to `true` for call `securityWorker` for this request */
  secure?: boolean
  /** request path */
  path: string
  /** content type of request body */
  type?: ContentType
  /** query params */
  query?: QueryParamsType
  /** format of response (i.e. response.json() -> format: "json") */
  format?: ResponseFormat
  /** request body */
  body?: unknown
  /** base url */
  baseUrl?: string
  /** request cancellation token */
  cancelToken?: CancelToken
}

export type RequestParams = Omit<FullRequestParams, 'body' | 'method' | 'query' | 'path'>

export interface ApiConfig<SecurityDataType = unknown> {
  baseUrl?: string
  baseApiParams?: Omit<RequestParams, 'baseUrl' | 'cancelToken' | 'signal'>
  securityWorker?: (
    securityData: SecurityDataType | null,
  ) => Promise<RequestParams | void> | RequestParams | void
  customFetch?: typeof fetch
}

export interface HttpResponse<D extends unknown, E extends unknown = unknown> extends Response {
  data: D
  error: E
}

type CancelToken = Symbol | string | number

export enum ContentType {
  Json = 'application/json',
  FormData = 'multipart/form-data',
  UrlEncoded = 'application/x-www-form-urlencoded',
  Text = 'text/plain',
}

export class HttpClient<SecurityDataType = unknown> {
  public baseUrl: string = ''
  private securityData: SecurityDataType | null = null
  private securityWorker?: ApiConfig<SecurityDataType>['securityWorker']
  private abortControllers = new Map<CancelToken, AbortController>()
  private customFetch = (...fetchParams: Parameters<typeof fetch>) => fetch(...fetchParams)

  private baseApiParams: RequestParams = {
    credentials: 'same-origin',
    headers: {},
    redirect: 'follow',
    referrerPolicy: 'no-referrer',
  }

  constructor(apiConfig: ApiConfig<SecurityDataType> = {}) {
    Object.assign(this, apiConfig)
  }

  public setSecurityData = (data: SecurityDataType | null) => {
    this.securityData = data
  }

  protected encodeQueryParam(key: string, value: any) {
    const encodedKey = encodeURIComponent(key)
    return `${encodedKey}=${encodeURIComponent(typeof value === 'number' ? value : `${value}`)}`
  }

  protected addQueryParam(query: QueryParamsType, key: string) {
    return this.encodeQueryParam(key, query[key])
  }

  protected addArrayQueryParam(query: QueryParamsType, key: string) {
    const value = query[key]
    return value.map((v: any) => this.encodeQueryParam(key, v)).join('&')
  }

  protected toQueryString(rawQuery?: QueryParamsType): string {
    const query = rawQuery || {}
    const keys = Object.keys(query).filter((key) => 'undefined' !== typeof query[key])
    return keys
      .map((key) =>
        Array.isArray(query[key])
          ? this.addArrayQueryParam(query, key)
          : this.addQueryParam(query, key),
      )
      .join('&')
  }

  protected addQueryParams(rawQuery?: QueryParamsType): string {
    const queryString = this.toQueryString(rawQuery)
    return queryString ? `?${queryString}` : ''
  }

  private contentFormatters: Record<ContentType, (input: any) => any> = {
    [ContentType.Json]: (input: any) =>
      input !== null && (typeof input === 'object' || typeof input === 'string')
        ? JSON.stringify(input)
        : input,
    [ContentType.Text]: (input: any) =>
      input !== null && typeof input !== 'string' ? JSON.stringify(input) : input,
    [ContentType.FormData]: (input: any) =>
      Object.keys(input || {}).reduce((formData, key) => {
        const property = input[key]
        formData.append(
          key,
          property instanceof Blob
            ? property
            : typeof property === 'object' && property !== null
              ? JSON.stringify(property)
              : `${property}`,
        )
        return formData
      }, new FormData()),
    [ContentType.UrlEncoded]: (input: any) => this.toQueryString(input),
  }

  protected mergeRequestParams(params1: RequestParams, params2?: RequestParams): RequestParams {
    return {
      ...this.baseApiParams,
      ...params1,
      ...(params2 || {}),
      headers: {
        ...(this.baseApiParams.headers || {}),
        ...(params1.headers || {}),
        ...((params2 && params2.headers) || {}),
      },
    }
  }

  protected createAbortSignal = (cancelToken: CancelToken): AbortSignal | undefined => {
    if (this.abortControllers.has(cancelToken)) {
      const abortController = this.abortControllers.get(cancelToken)
      if (abortController) {
        return abortController.signal
      }
      return void 0
    }

    const abortController = new AbortController()
    this.abortControllers.set(cancelToken, abortController)
    return abortController.signal
  }

  public abortRequest = (cancelToken: CancelToken) => {
    const abortController = this.abortControllers.get(cancelToken)

    if (abortController) {
      abortController.abort()
      this.abortControllers.delete(cancelToken)
    }
  }

  public request = async <T = any, E = any>({
    body,
    secure,
    path,
    type,
    query,
    format,
    baseUrl,
    cancelToken,
    ...params
  }: FullRequestParams): Promise<T> => {
    const secureParams =
      ((typeof secure === 'boolean' ? secure : this.baseApiParams.secure) &&
        this.securityWorker &&
        (await this.securityWorker(this.securityData))) ||
      {}
    const requestParams = this.mergeRequestParams(params, secureParams)
    const queryString = query && this.toQueryString(query)
    const payloadFormatter = this.contentFormatters[type || ContentType.Json]
    const responseFormat = format || requestParams.format

    return this.customFetch(
      `${baseUrl || this.baseUrl || ''}${path}${queryString ? `?${queryString}` : ''}`,
      {
        ...requestParams,
        headers: {
          ...(requestParams.headers || {}),
          ...(type && type !== ContentType.FormData ? { 'Content-Type': type } : {}),
        },
        signal: (cancelToken ? this.createAbortSignal(cancelToken) : requestParams.signal) || null,
        body: typeof body === 'undefined' || body === null ? null : payloadFormatter(body),
      },
    ).then(async (response) => {
      const r = response.clone() as HttpResponse<T, E>
      r.data = null as unknown as T
      r.error = null as unknown as E

      const data = !responseFormat
        ? r
        : await response[responseFormat]()
            .then((data) => {
              if (r.ok) {
                r.data = data
              } else {
                r.error = data
              }
              return r
            })
            .catch((e) => {
              r.error = e
              return r
            })

      if (cancelToken) {
        this.abortControllers.delete(cancelToken)
      }

      if (!response.ok) throw data
      return data.data
    })
  }
}

/**
 * @title Juicer
 * @version 0.0.0
 *
 * Api schema of juicer device
 */
export class Api<SecurityDataType extends unknown> extends HttpClient<SecurityDataType> {
  api = {
    /**
     * No description
     *
     * @name PreflightRequest
     * @summary preflight request
     * @request OPTIONS:/api
     */
    preflightRequest: (params: RequestParams = {}) =>
      this.request<null, any>({
        path: `/api`,
        method: 'OPTIONS',
        ...params,
      }),

    /**
     * No description
     *
     * @tags ap, wifi
     * @name GetApCredentials
     * @summary get ap credentials
     * @request GET:/api/wifi/ap
     */
    getApCredentials: (params: RequestParams = {}) =>
      this.request<WifiCredentials, any>({
        path: `/api/wifi/ap`,
        method: 'GET',
        format: 'json',
        ...params,
      }),

    /**
     * No description
     *
     * @tags ap, wifi
     * @name SetApCredentials
     * @summary set ap credentials
     * @request PATCH:/api/wifi/ap
     */
    setApCredentials: (data: WifiCredentials, params: RequestParams = {}) =>
      this.request<string, any>({
        path: `/api/wifi/ap`,
        method: 'PATCH',
        body: data,
        type: ContentType.UrlEncoded,
        ...params,
      }),

    /**
     * No description
     *
     * @tags wifi
     * @name GetWifiState
     * @summary get wifi state
     * @request GET:/api/wifi/state
     */
    getWifiState: (params: RequestParams = {}) =>
      this.request<
        {
          ip: string
          mode: WifiMode
          rssi: number
        },
        any
      >({
        path: `/api/wifi/state`,
        method: 'GET',
        format: 'json',
        ...params,
      }),

    /**
     * No description
     *
     * @tags network, wifi
     * @name GetNetworkApCredentials
     * @summary get network ap credentials
     * @request GET:/api/wifi/network
     */
    getNetworkApCredentials: (params: RequestParams = {}) =>
      this.request<WifiCredentials, any>({
        path: `/api/wifi/network`,
        method: 'GET',
        format: 'json',
        ...params,
      }),

    /**
     * No description
     *
     * @tags network, wifi
     * @name SetNetworkApCredentials
     * @summary set network ap credentials
     * @request PATCH:/api/wifi/network
     */
    setNetworkApCredentials: (data: WifiCredentials, params: RequestParams = {}) =>
      this.request<string, any>({
        path: `/api/wifi/network`,
        method: 'PATCH',
        body: data,
        type: ContentType.UrlEncoded,
        ...params,
      }),

    /**
     * No description
     *
     * @tags network
     * @name DelNetworkApCredentials
     * @summary del network ap credentials
     * @request DELETE:/api/wifi/network
     */
    delNetworkApCredentials: (params: RequestParams = {}) =>
      this.request<string, any>({
        path: `/api/wifi/network`,
        method: 'DELETE',
        ...params,
      }),

    /**
     * No description
     *
     * @tags tds
     * @name GetTdsSensorValue
     * @summary Get tds sensor value
     * @request GET:/api/tds
     */
    getTdsSensorValue: (params: RequestParams = {}) =>
      this.request<string, any>({
        path: `/api/tds`,
        method: 'GET',
        ...params,
      }),

    /**
     * No description
     *
     * @tags update
     * @name GetFirmwareVersion
     * @summary get firmware version
     * @request GET:/api/update/fw
     */
    getFirmwareVersion: (params: RequestParams = {}) =>
      this.request<string, any>({
        path: `/api/update/fw`,
        method: 'GET',
        ...params,
      }),

    /**
     * No description
     *
     * @tags update
     * @name UploadFirmware
     * @summary upload firmware
     * @request POST:/api/update/fw
     */
    uploadFirmware: (data: object, params: RequestParams = {}) =>
      this.request<string, any>({
        path: `/api/update/fw`,
        method: 'POST',
        body: data,
        type: ContentType.FormData,
        ...params,
      }),

    /**
     * No description
     *
     * @tags update
     * @name UploadFilesystem
     * @summary upload filesystem
     * @request POST:/api/update/fs
     */
    uploadFilesystem: (data: object, params: RequestParams = {}) =>
      this.request<string, any>({
        path: `/api/update/fs`,
        method: 'POST',
        body: data,
        type: ContentType.FormData,
        ...params,
      }),
  }
}
