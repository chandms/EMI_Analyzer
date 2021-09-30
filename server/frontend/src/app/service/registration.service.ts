import { Injectable } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { environment as env } from 'src/environments/environment';
import { Register } from '../registration/Register';
import { User } from '../login/User';
import { LocationStrategy } from '@angular/common';

@Injectable({
  providedIn: 'root'
})
export class RegistrationService {

  private registration_url = env.backend_url + '/create_user';

  constructor(private http: HttpClient,private locationStrategy: LocationStrategy) { }
  user: User;
  decodeUser(username: string ,password: string, firstname: string, lastname: string, email: string){
    let body = {
      'username': username,
      'password': password,
      'firstname': firstname,
      'lastname': lastname,
      'email': email

    };
    return this.http.post<Register>(this.registration_url, body);
  }
}
