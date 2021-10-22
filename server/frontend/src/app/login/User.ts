export interface User{
    iat: number,
    exp: number,
    firstname: string,
    lastname: string,
    email: string,
    username: string,
    password: string
}

export class User implements User{
	iat: number =0;
    exp: number =0;
    firstname: string ='';
    lastname: string ='';
    email: string ='';
    username: string='';
    password: string='';

}