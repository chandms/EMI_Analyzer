import { HttpClient, HttpParams } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment as env } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class SweepService {

  private url = env.sweep_url;

  constructor(private http: HttpClient) { }

  getAllSweeps() {
    return this.http.get(this.url);
  }

  getLatestSweep(){
    let params = new HttpParams;
    params = params.append('latest', 'true')
    return this.http.get(this.url, {params})
  }

}
